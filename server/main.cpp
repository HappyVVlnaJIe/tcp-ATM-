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

std::pair<Request, std::vector<int>> ParseComand(char* buff) {
    std::pair<Request, std::vector<int>> result;
    //char * mas = strtok(buff," "); через это сделать
    std::string comand;
    std::stringstream ss;
    ss<<buff;
    ss>>comand;
    result.first = request_translate[comand];//добавить проверку на существтвание
    switch (result.first)
    {
    case Request::login:
        result.second.resize(2);
        ss>>result.second[0]>>result.second[1];
        break;
    case Request::add:
        result.second.resize(1);
        ss>>result.second[0];
        /* code */
        break;
    case Request::help:
        /* code */
        break;
    case Request::lock:
        /* code */
        break;
    case Request::unlock:
        /* code */
        break;
    case Request::status:
        /* code */
        break;
    case Request::withdrawal:
        result.second.resize(1);
        ss>>result.second[0];
        /* code */
        break;
        
    default://custom exception
        break;
    }

    return result;
}

int main(void)
{
    char sendBuff[1025];
    Database db("./cash-machine-users.txt");
    struct sockaddr_in sa;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (SocketFD == -1) {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }
    
    memset(&sa, 0, sizeof sa);
    memset(sendBuff, '0', sizeof(sendBuff));

    sa.sin_family = AF_INET;
    sa.sin_port = htons(5000);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(SocketFD,(struct sockaddr *)&sa, sizeof(sa)) == -1) {
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
        int ConnectFD = accept(SocketFD, NULL, NULL);
        read(ConnectFD,sendBuff,sizeof(sendBuff));
        std::cout<<sendBuff<<std::endl;
        std::pair<Request, std::vector<int>> pair=ParseComand(sendBuff); 
        bool is_login=false;
        int card_number;
        while(pair.first!=Request::logout) {
            switch (pair.first)
            {
            case Request::add:
                if (!is_login) {
                    snprintf(sendBuff, sizeof(sendBuff), "%s", "you are not authorized ");
                    write(ConnectFD, sendBuff, strlen(sendBuff));
                    break;
                }
                //db.Add(card_number,pair.second[0]);
                snprintf(sendBuff, sizeof(sendBuff), "%s", db.Add(card_number,pair.second[0]).c_str());
                write(ConnectFD, sendBuff, strlen(sendBuff));
                std::cout<<"write done";
                /* code */
                break;
            case Request::help:
                /* code */
                break;
            case Request::lock:
                if (!is_login) {
                    snprintf(sendBuff, sizeof(sendBuff), "%s", "you are not authorized ");
                    write(ConnectFD, sendBuff, strlen(sendBuff));
                }
                /* code */
                break;
            case Request::login:
                /* code */
                break;
            case Request::logout:
                /* code */
                break;
            case Request::status:
                if (!is_login) {
                    snprintf(sendBuff, sizeof(sendBuff), "%s", "you are not authorized ");
                    write(ConnectFD, sendBuff, strlen(sendBuff));
                }
                /* code */
                break;
            case Request::unlock:
                if (!is_login) {
                    snprintf(sendBuff, sizeof(sendBuff), "%s", "you are not authorized ");
                    write(ConnectFD, sendBuff, strlen(sendBuff));
                }
                /* code */
                break;
            case Request::withdrawal:
                if (!is_login) {
                    snprintf(sendBuff, sizeof(sendBuff), "%s", "you are not authorized ");
                    write(ConnectFD, sendBuff, strlen(sendBuff));
                }
                /* code */
                break;
            
            default:
                break;
            }
            
            read(ConnectFD,sendBuff,sizeof(sendBuff));
            std::pair<Request, std::vector<int>> pair=ParseComand(sendBuff); 
            //std::cout<<sendBuff<<std::endl;
            //time_t ticks = time(NULL);
            //snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
            //write(ConnectFD, sendBuff, strlen(sendBuff));
        }
        close(ConnectFD);
        sleep(1);
    }

    close(SocketFD);
    return EXIT_SUCCESS;  
}