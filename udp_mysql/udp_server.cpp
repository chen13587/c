#include "udp_server.h"
#include "mysql_server.h"
using namespace std;
_sock_para sock_para;
socklen_t cli_len= sizeof(sock_para.cli);
pid_t send_pid=0;

void send_task(){
    int tmp=0;
    struct sockaddr_in c;           //用于接收管道指针
    socklen_t c_len=0;
    c_len=sizeof(c);
    printf("等待接收客户端socket....\n");
    read(sock_para.sock_pipe[0],(void*)&c,(size_t)c_len);
//      c=&tmp;
    printf("拿到客户端socket\n");
    for(;;){
        memset(sock_para.send_buf,0,sizeof(sock_para.send_buf));
        cin>>sock_para.send_buf;
        printf("手动发送：%s\n",sock_para.send_buf);
        if(!strncmp("exit",(char*)sock_para.send_buf,strlen("exit")))
            break;
        sendto(sock_para.sd,&sock_para.send_buf,strlen((char*)sock_para.send_buf),0,(struct sockaddr*)&c,c_len);
    }
}

//extern _mysql sql_para;
void udp_server_task(uint16_t port){
    pipe(sock_para.sock_pipe);

    sock_para.sd=socket(AF_INET,SOCK_DGRAM,0);
    if(0>sock_para.sd){
        cout<<"create socket error"<<endl;
        return ;
    }
    sock_para.addr.sin_family=AF_INET;
    //  addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    sock_para.addr.sin_port=htons(port);

    if(0>bind(sock_para.sd,(sockaddr*)&sock_para.addr,sizeof(sock_para.addr))) {
        printf("bind error\r\n");
        return ;
    }

    send_pid=fork();
    if(send_pid==0){
        send_task();
    }
    for(;;){
        memset(sock_para.recv_buf,0,BUF_SIZE);
        sock_para.recv_len=recvfrom(sock_para.sd,&sock_para.recv_buf,BUF_SIZE,0,(struct sockaddr*)&sock_para.cli,&cli_len);
        write(sock_para.sock_pipe[1],(void*)&sock_para.cli,(size_t)cli_len);
        write(sql_para.sql_pipe[1],(void*)&sock_para.recv_buf,(size_t)sock_para.recv_len);

        //printf("\n-------->\n%s\n---->\n",sock_para.recv_buf);
        printf("\n----->\n");
        for(int i=0;i<sock_para.recv_len;i++)
            printf("%02x ",sock_para.recv_buf[i]);
        printf("\n-------->\n");

        sock_para.send_len=sprintf((char*)&sock_para.send_buf[0],"your sent: <%s>\n",sock_para.recv_buf);
        sendto(sock_para.sd,&sock_para.send_buf,sock_para.send_len,0,(struct sockaddr*)&sock_para.cli,cli_len);
        if(0>sock_para.recv_len)
            break;
    }
    close(sock_para.sd);
    cout << "disconnect" <<endl;



}

