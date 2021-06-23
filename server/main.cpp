#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include "database.h"
#include <cstring>
#include <sstream> 


#define DEFAULT_PORT 5000
#define BUFFER_SIZE 1024

struct client {
    int card_number;
    bool is_login;
};

void ParseComand(char* buff, Database& db, client& client) {
    //char * mas = strtok(buff," "); через это сделать
    std::string comand;
    std::stringstream ss;
    ss<<buff;
    ss>>comand;
    int card_number, pin, funds;
    Request request= request_translate[comand];//добавить проверку на существтвание

    if (!client.is_login&&request!=Request::login) {
        strcpy(buff,"you need to log in ");
    }
    
    if (!client.is_login) {
        ss>>card_number>>pin;
        if (db.Login(card_number, pin)) {
            strcpy(buff,"logged in");
            client.card_number=card_number;
            client.is_login=true;
            return;
        }
        strcpy(buff,"wrong card number or pin ");
    }

    switch (request)
    {
    case Request::add:
        ss<<funds;
        try
        {
            db.Add(client.card_number, funds);
        }
        catch(const std::exception& e)
        {
            strcpy(buff,e.what());
        }
        break;
    case Request::lock:
        try
        {
            db.Lock(client.card_number);
        }
        catch(const std::exception& e)
        {
             strcpy(buff,e.what());
        }
        
        break;
    case Request::unlock:
        try
        {
            db.Unlock(client.card_number);
        }
        catch(const std::exception& e)
        {
             strcpy(buff,e.what());
        }
        break;
    case Request::status:
        try
        {
            db.Unlock(client.card_number);
        }
        catch(const std::exception& e)
        {
             strcpy(buff,e.what());
        }
        break;
    case Request::withdrawal:
        try
        {
            db.Unlock(client.card_number);
        }
        catch(const std::exception& e)
        {
             strcpy(buff,e.what());
        }
        break;
        
    case Request::logout:
        break;
    default:                    //custom exception
        break;
    }

}

int main(int argc, char const* argv[])
{
    Database db("./cash-machine-users.txt");
    struct sockaddr_in server_address;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (SocketFD == -1) {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(SocketFD,(struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    
    if (listen(SocketFD, 10) == -1) {
        perror("listen failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    
    while (true) {
        int server = accept(SocketFD, NULL, NULL), n;

        if (server<0) {
            std::cout<<"Can't accepting client.\n";
        }

        char buffer[BUFFER_SIZE];


        strcpy(buffer,"Server conected");
        //std::cout<<"send message"<<std::endl;
        send(server, buffer, BUFFER_SIZE,0);
        //std::cout<<"message send"<<std::endl;
        client client;
        client.is_login=false;                  // add constructor
        while (server>0) {
            recv(server,buffer, BUFFER_SIZE, 0);
            ParseComand(buffer, db, client);
            if (buffer=="logout") {
                std::cout<<"logout"<<std::endl;
                break;
            }
            std::cout<<buffer<<std::endl;
            strcpy(buffer,"send");
            send(server,buffer,BUFFER_SIZE,0);
        }
        std::cout<<"out of while"<<std::endl;
        strcpy(buffer,"goodbye");
        send(server,buffer, BUFFER_SIZE, 0);
        close(server);
        sleep(1);
    }

    close(SocketFD);
    return EXIT_SUCCESS;  
}