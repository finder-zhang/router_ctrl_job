#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

/*
串口程序与路由器程序交互的demo

考虑到更加方便开发调试，以及扩展性，个人觉得用socket方式通信最好。

开发时，该程序可直接在ubuntu开发机上执行，只要设置router_app_host即可。
*/

//通过socket连接路由器程序
int local_socket_init(char* host,int port){
    int sockfd;
    struct sockaddr_in addr;
    
    memset(&addr,0,sizeof(struct sockaddr_in));
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<=0){
        fprintf(stderr,"create socket error\n");
        return -1;
    }
    addr.sin_family=AF_INET;
    addr.sin_port = htons(port);
    inet_aton(host,&addr.sin_addr);
    if(connect(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in))<0){
        fprintf(stderr,"connect to %s:%d fail\n",host,port);
        close(sockfd);
        return -1;
    }
    return sockfd;
}



void main(int argc,char* argv[]){
    int sockfd;
    FILE* sockFile;
    char buffer[1024];
    
    char* router_app_host="192.168.1.254"; //路由器IP
    int router_app_port=8767; //路由器程序与串口程序通信的端口
    
    //连接路由器程序
    sockfd=local_socket_init(router_app_host,router_app_port);
    if(sockfd<0){
        return;
    }
    sockFile=fdopen(sockfd,"r+");
    
    while(fgets(buffer,1024,sockFile)){
        printf("收到路由器发来的控制命令: %s\n",buffer);
        //这儿要对路由器发来的命令分析，并控制串口，控制完成后发路由器程序发送设备的最新状态
        //需要定制具体通信协议。
    }
}
