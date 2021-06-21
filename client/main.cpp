#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <iostream>


int main(void) {
	struct sockaddr_in stSockAddr;
	int i32Res;
	int i32SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (i32SocketFD == -1) {
		perror("Ошибка: невозможно создать сокет");
		return EXIT_FAILURE;
	}

	memset(&stSockAddr, 0, sizeof (stSockAddr));

	stSockAddr.sin_family = PF_INET;
	stSockAddr.sin_port = htons(5000);
	i32Res = inet_pton(PF_INET, "127.0.0.1", &stSockAddr.sin_addr);

	if (i32Res < 0) {
		perror("Ошибка: первый параметр не относится к категории корректных адресов");
		close(i32SocketFD);
		return EXIT_FAILURE;
	} else if (!i32Res) {
		perror("Ошибка: второй параметр не содержит корректного IP-адреса");
		close(i32SocketFD);
		return EXIT_FAILURE;
	}

	if (connect(i32SocketFD, (struct sockaddr*) &stSockAddr, sizeof (stSockAddr)) == -1) {
		perror("Ошибка: соединения");
		close(i32SocketFD);
		return EXIT_FAILURE;
	}

	/* выполнение операций чтения и записи ... */

    char buff[1024];
    memset(buff,'0',sizeof(buff));
    int n=0; 
    std::string comand;
    while(true) {	
        std::getline(std::cin, comand);
        snprintf(buff,sizeof(buff),"%s",comand.c_str());
        write(i32SocketFD,buff,sizeof(buff));
        std::cout<<"write done"<<std::endl;
        while ( (n = read(i32SocketFD, buff, sizeof(buff)-1)) > 0)
        {
            buff[n] = 0;
            if(fputs(buff, stdout) == EOF)
            {
                printf("\n Error : Fputs error\n");
            }
        }

        if(n < 0)
        {
            printf("\n Read error \n");
        }
    }
	shutdown(i32SocketFD, SHUT_RDWR);

	close(i32SocketFD);
	return 0;
}