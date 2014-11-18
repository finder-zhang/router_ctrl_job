#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


//const char*		PATH_PIPE_SOCKET = "~/tmpPipeSock";
const char*		PATH_PIPE_SOCKET = "/root/tmpPipeSock";
const int			SOCKET_LISTEN_COUNT = 4;

int local_socket_create(char* name){
    struct sockaddr_un addr;
    int sockfd;

    //unlink(name);
    memset(&addr,0,sizeof(addr));
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, name);
    sockfd=socket(AF_LOCAL,SOCK_STREAM, 0);
    if ( -1 == sockfd ) {
    	return -1;
    }
    bind(sockfd,(struct sockaddr*) &addr,sizeof(addr));
    listen(sockfd,SOCKET_LISTEN_COUNT);
    return sockfd;
}


int _main(int argc,char* argv[]){
    int cs;
    FILE* css;
    char buffer[1024];
    
    int socket=local_socket_create("/root/tmpPipeSock");
    //int socket=local_socket_create(PATH_PIPE_SOCKET);
    
    //创建子进程
    //system("./child ./pipe.sock &");
    system("/root/workspace/router_proj/zbc/Debug/zbc /root/tmpPipeSock &");


    
    //测试程序就不做异步之类的操作了。
    cs=accept(socket,NULL,0);
    css=fdopen(cs,"r");
    
    //fgets(buffer,1024,css);
    recv(cs,buffer,1024,0);
    printf("%s",buffer);
    fgets(buffer,1024,css);
    printf("%s",buffer);
    
    send(cs,"list\n",5,0);
    fgets(buffer,1024,css);
    printf("parent recieved: %s\n",buffer);
    send(cs,"exit\n",5,0);
    close(socket);

    return 0;
}
