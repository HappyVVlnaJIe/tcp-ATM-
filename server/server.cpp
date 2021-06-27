#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread> 
#include <errno.h>

Client::Client(int descriptor) : descriptor(descriptor){} 

std::string Server::ServerMessage(Message message) {
    return server_message[message];
}

void Server::SetMessage() {
    server_message = {
        {Message::CONECTED, "Server conected"},
        {Message::DISCONNECTED, "Goodbye"},
        {Message::NOT_LOG_IN, "You need to log in"},
        {Message::ALREADY_CONNECTED, "You are already connected"},
        {Message::COMPLETE, "Complete"},
        {Message::ALREADY_LOG_IN, "You are already logged in"},
    };
}

Server::Server(int port, int buffer_size) {
    this->SetMessage();
    this->buffer_size=buffer_size;
    struct sockaddr_in server_address;
    socketFD = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);
    if (socketFD < 0) {
        std::cout<<"cannot create socket"<<std::endl;
        exit(0);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(socketFD,reinterpret_cast<struct sockaddr*>(&server_address), 
            sizeof(server_address))<0) {
        std::cout<<"bind failed"<<std::endl;
        close(socketFD);
        exit(0);
    }
    
    if (listen(socketFD, 10) == -1) {
        std::cout<<"listen failed"<<std::endl;
        close(socketFD);
        exit(0);
    }
}

std::string Server::Recv(int descriptor) {
    std::string message="";
    char buffer[buffer_size]; 
    int n;
    while((n=recv(descriptor,buffer,buffer_size,MSG_DONTWAIT))>=0){
        message+=buffer;
    }
    return message;
}

bool Server::AddActiveCard(int card_number) {
    return active_cards.insert(card_number).second;
}

int Server::Send(std::string message, int descriptor) {
    return send(descriptor,message.c_str(),message.size(),0);
}

void Server::Start() {

    while(true) {
        if (clients.size()==0) {
            Client client;
            while((client.descriptor=accept(socketFD,NULL,NULL))<0){};                  
            this->Send(ServerMessage(Message::CONECTED), client.descriptor);
            clients.push_back(client);
        }
        int descriptor=-1;
        descriptor=accept(socketFD,NULL,NULL);
        if (descriptor>0) {
            clients.emplace_back(descriptor);
            this->Send(ServerMessage(Message::CONECTED), descriptor);
        }
        for (int i=0;i<clients.size();i++) {
            auto& client = clients[i];
            if (client.descriptor<0) {
                close(client.descriptor);
                active_cards.erase(client.card_number);
                clients.erase(clients.begin()+i);
                continue;
            }
            std::string message=this->Recv(client.descriptor);
            if (message!="") {
                try
                {
                    message=this->ProcessRequest(message, client);
                    //std::cout<<"Proccess request "<<message<<std::endl;
                    this->Send(message, client.descriptor);
                    if (message==ServerMessage(Message::DISCONNECTED)) {
                        close(client.descriptor);
                        active_cards.erase(client.card_number);
                        clients.erase(clients.begin()+i);
                    }
                }
                catch(const std::exception& e)
                {
                    this->Send(e.what(), client.descriptor);
                }
            }
        }
        std::this_thread::yield();
    }
}