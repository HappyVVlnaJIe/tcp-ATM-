#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <errno.h>

Client::Client(int descriptor) : descriptor(descriptor) {}

std::string Server::ServerMessage(Message message)
{
    return server_message[message];
}

void Server::SetMessage()
{
    server_message = {
        {Message::CONNECTED, "Server connected"},
        {Message::DISCONNECTED, "Goodbye"},
        {Message::NOT_LOG_IN, "You need to log in"},
        {Message::ALREADY_CONNECTED, "You are already connected"},
        {Message::COMPLETE, "Complete"},
        {Message::ALREADY_LOG_IN, "You are already logged in"},
    };
}

Server::Server(int port, int buffer_size)
{
    this->SetMessage();
    this->buffer_size = buffer_size;
    struct sockaddr_in server_address;
    socketFD = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (socketFD < 0)
    {
        std::cout << "cannot create socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socketFD, reinterpret_cast<struct sockaddr *>(&server_address),
             sizeof(server_address)) < 0)
    {
        std::cout << "bind failed" << std::endl;
        close(socketFD);
        exit(EXIT_FAILURE);
    }

    if (listen(socketFD, 10) == -1)
    {
        std::cout << "listen failed" << std::endl;
        close(socketFD);
        exit(EXIT_FAILURE);
    }
}

std::string Server::Recv(int descriptor)
{
    std::string message = "";
    char buffer[buffer_size];
    int n;
    while ((n=recv(descriptor, buffer, buffer_size, MSG_DONTWAIT)) > 0)
    {
        message += buffer;
    }
    if (n==0) {
        throw ClientDisconnectError();
    }
    return message;
}

bool Server::AddActiveCard(int card_number)
{
    return active_cards.insert(card_number).second;
}

int Server::Send(std::string message, int descriptor)
{
    return send(descriptor, message.c_str(), message.size()+1, 0);
}

void Server::AddClient() {
    int descriptor = -1;
    descriptor = accept(socketFD, NULL, NULL);
    if (descriptor > 0) {
        clients.emplace_back(descriptor);
        this->Send(ServerMessage(Message::CONNECTED), descriptor);
    }
}

void Server::Start()
{
    while (true) {
        AddClient();
        for (auto it=clients.begin(); it != clients.end(); it++) {
            std::string recv_message="";
            try
            {
                recv_message = this->Recv(it->descriptor);
            }
            catch(const std::exception& e)
            {
                close(it->descriptor);
                active_cards.erase(it->card_number);
                it=clients.erase(it);
            }
            if (recv_message != "") {
                try
                {
                    std::string send_message;
                    send_message = this->ProcessRequest(recv_message, *it);
                    this->Send(send_message, it->descriptor);
                    if (send_message == ServerMessage(Message::DISCONNECTED))
                    {
                        close(it->descriptor);
                        active_cards.erase(it->card_number);
                        it=clients.erase(it);
                    }
                }
                catch (const std::exception &e)
                {
                    this->Send(e.what(), it->descriptor);
                }
            }
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }
}