#include "server.h"

Client::Client(int descriptor) : descriptor(descriptor){} 

Server::Server(int port, int buffer_size) {
    struct sockaddr_in server_address;
    socketFD = socket(PF_INET, SOCK_STREAM|SOCK_NONBLOCK, IPPROTO_TCP);
    if (socketFD == -1) {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(socketFD,(struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind failed");
        close(socketFD);
        exit(EXIT_FAILURE);
    }
    
    if (listen(socketFD, 10) == -1) {
        perror("listen failed");
        close(socketFD);
        exit(EXIT_FAILURE);
    }
}

std::string Server::Recv(int descriptor) {
    std::string message="";
    char buffer[buffer_size]; 
    while(recv(descriptor,buffer,buffer_size,0)>0){
        message+=buffer;
    }
    return message;
}

int Server::Send(std::string message, int descriptor) {
    send(descriptor,message.c_str(),message.size(),0);
}

void Server::Start() {
    while(true) {
        if (clients.size()==0) {
            Client client;
            while(client.descriptor=accept(socketFD,NULL,NULL));
        }
        int descriptor;
        descriptor=accept(socketFD,NULL,NULL);
        if (descriptor>=0) {
            clients.push_back(Client(descriptor));
        }
        for (auto& client : clients) {
            std::string message=this->Recv(client.descriptor);
            if (message!="") {
                this->Send(this->ProcessRequest(message, client), client.descriptor);
            }
        }
        std::this_thread::yield();
    }
}