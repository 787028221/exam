#include <stdio.h>   
#include <string.h>   
#include <unistd.h>   
#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <arpa/inet.h>   
#include <sys/time.h>   
#include <stdlib.h>   
#include "ex_db.h"

#define PORT 1234               //服务器端口   
#define BACKLOG 5               //listen队列中等待的连接数   
#define MAXDATASIZE 4096        //缓冲区大小   


typedef struct _CLIENT  
{  
    int fd;                     //客户端socket描述符   
    char name[20];              //客户端名称   
    struct sockaddr_in addr;    //客户端地址信息结构体   
    char data[MAXDATASIZE];     //客户端私有数据指针   
} CLIENT;

enum {
    LOGIN_CMD,
    REG_CMD,
    INFO_CMD,
    PAPER_CMD,
} CMD_T;

void process_client(CLIENT * client, char *recvbuf, int len, int type);   //客户请求处理函数   
void main(int argc ,char **argv)  
{  
    int i, maxi, maxfd, sockfd;  
    int nready;  
    ssize_t n;  
    fd_set rset, allset;        //select所需的文件描述符集合   
    int listenfd, connectfd;    //socket文件描述符   
    struct sockaddr_in server;  //服务器地址信息结构体   
  
    CLIENT client[FD_SETSIZE];  //FD_SETSIZE为select函数支持的最大描述符个数   
    char recvbuf[MAXDATASIZE];  //缓冲区   
    int sin_size;               //地址信息结构体大小   
  
    printf("Init db!\r\n");
    init_db();
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)  
    {                           //调用socket创建用于监听客户端的socket   
        perror("Creating socket failed.");  
        exit(1);  
    }  
  
    int opt = SO_REUSEADDR;  
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  //设置socket属性   
  
    bzero(&server, sizeof(server));  
    server.sin_family = AF_INET;  
    server.sin_port = htons(PORT);  
    server.sin_addr.s_addr = htonl(INADDR_ANY);  
  
    if (bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)  
    {                           //调用bind绑定地址   
        perror("Bind error.");  
        exit(1);  
    }  
  
    if (listen(listenfd, BACKLOG) == -1)  
    {                           //调用listen开始监听   
        perror("listen() error\n");  
        exit(1);  
    }  
  
    //初始化select   
    maxfd = listenfd;
    maxi = -1;  
    for (i = 0; i < FD_SETSIZE; i++)  
    {  
        client[i].fd = -1;  
    }  
    FD_ZERO(&allset);           //清空   
    FD_SET(listenfd, &allset);  //将监听socket加入select检测的描述符集合   
  
    while (1)  
    {  
        struct sockaddr_in addr;  
        rset = allset;
        memset(recvbuf, 0, sizeof(recvbuf));
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);    //调用select   
        printf("Select() break and the return num is %d. \n", nready);  
  
        if (FD_ISSET(listenfd, &rset))  
        {                       //检测是否有新客户端请求   
            printf("Accept a connection.\n");  
            //调用accept，返回服务器与客户端连接的socket描述符   
            sin_size = sizeof(struct sockaddr_in);  
            if ((connectfd =  
                 accept(listenfd, (struct sockaddr *)&addr, (socklen_t *) & sin_size)) == -1)  
            {  
                perror("Accept() error\n");  
                continue;  
            }  
  
            //将新客户端的加入数组   
            for (i = 0; i < FD_SETSIZE; i++)  
            {  
                if (client[i].fd < 0)  
                {  
                    char buffer[20];  
                    client[i].fd = connectfd;   //保存客户端描述符   
                    memset(buffer, '0', sizeof(buffer));  
                    sprintf(buffer, "Client[%.2d]", i);  
                    memcpy(client[i].name, buffer, strlen(buffer));  
                    client[i].addr = addr;  
                    memset(buffer, '0', sizeof(buffer));  
                    sprintf(buffer, "Only For Test!");  
                    memcpy(client[i].data, buffer, strlen(buffer));  
                    printf("You got a connection from %s:%d.\n", inet_ntoa(client[i].addr.sin_addr),ntohs(client[i].addr.sin_port));  
                    printf("Add a new connection:%s\n",client[i].name);  
                    break;  
                }  
            }  
              
            if (i == FD_SETSIZE)  
                printf("Too many clients\n");  
            FD_SET(connectfd, &allset); //将新socket连接放入select监听集合   
            if (connectfd > maxfd)  
                maxfd = connectfd;  //确认maxfd是最大描述符   
            if (i > maxi)       //数组最大元素值   
                maxi = i;  
            if (--nready <= 0)  
                continue;       //如果没有新客户端连接，继续循环   
        }  
  
        for (i = 0; i <= maxi; i++)  
        {  
            if ((sockfd = client[i].fd) < 0)    //如果客户端描述符小于0，则没有客户端连接，检测下一个   
                continue;  
            // 有客户连接，检测是否有数据   
            if (FD_ISSET(sockfd, &rset))  
            {  
                printf("Receive from connect fd[%d].\n", i);  
                if ((n = recv(sockfd, recvbuf, MAXDATASIZE, 0)) == 0)  
                {               //从客户端socket读数据，等于0表示网络中断   
                    close(sockfd);  //关闭socket连接   
                    printf("%s closed. User's data: %s\n", client[i].name, client[i].data);  
                    FD_CLR(sockfd, &allset);    //从监听集合中删除此socket连接   
                    client[i].fd = -1;  //数组元素设初始值，表示没客户端连接   
                }  
                else { 
		    int type = 0;
                    process_client(&client[i], recvbuf, n, type); //接收到客户数据，开始处理   
		}

                if (--nready <= 0)  
                    break;      //如果没有新客户端有数据，跳出for循环回到while循环   
            }  
        }  
    }  
    close(listenfd);            //关闭服务器监听socket        
}

void read_question_db()
{

}

/************************************************* 
* Function    : generate_exam() 
* Description : 自动组卷程序
* Calls       :  
* Called By   : main() 
* Input       :  
* Output      :  
* Return      : 
*************************************************/  
void generate_exam()
{

}

/************************************************* 
* Function    : process_client() 
* Description : 处理客户端连接函数 
* Calls       :  
* Called By   : main() 
* Input       :  
* Output      :  
* Return      : 
*************************************************/  
void process_client(CLIENT * client, char *recvbuf, int len, int type)  
{  
    char sendbuf[MAXDATASIZE] = {0}; 
    char buf[MAXDATASIZE];
    char *p_str;
    char *key_point;
    char pkt_buf[4096] = {0};
    char msg[8][4096] = {0, 0};
    int i;  
    int ret;
    char *str = NULL;
  
    printf("Received client( %s ) message: %s\n", client->name, recvbuf);  

    memcpy(buf, recvbuf, len);
    buf[len] = '\0';
    p_str = buf;
    int round = 0;
    while (NULL != (key_point = strsep(&p_str, "|"))) {
        if (round < 8) {
            strncpy(msg[round], key_point, strlen(key_point));
            printf("key_point[%d] %s--<%s>\r\n", round, key_point, msg[round]);
        } else {
            break;
        }
        round++;
    }


    if (strncmp(msg[0], "02", strlen("02")) == 0) {
        printf("student login check!\r\n");
        ret = serach_stu(msg[1], msg[2]);
        if (ret == 0) {
            str = "EA021EB";
        } else if (ret == -2){
            str = "EA022EB";
        } else {
            str = "EA020EB";
        }
    } else if (strncmp(msg[0], "00", strlen("00")) == 0) {
        printf("admin login check\r\n");
        ret = serach_admin(msg[1], msg[2]);
        if (ret == 0) {
            printf("check admin ok\r\n");
            str = "EA001EB";
        } else if (ret == -2){
            str = "EA002EB";
        } else {
            str = "EA000EB";
        }
    } else if (strncmp(msg[0], "01", strlen("01")) == 0) {
        printf("teacher login check!\r\n");
        ret = serach_teacher(msg[1], msg[2]);
        if (ret == 0) {
            printf("check teacher ok\r\n");
            str = "EA011EB";
        } else if (ret == -2){
            str = "EA012EB";
        } else {
            str = "EA010EB";
        }
    } else if (strncmp(msg[0], "10", strlen("10")) == 0) {
        printf("Reg admin %s--<%s>\r\n", base64_decode(msg[1], strlen(msg[1])), msg[1]);
        ret = reg_admin(base64_decode(msg[1], strlen(msg[1])), msg[2], msg[3]);
        if (ret == 0) {
            printf("Reg admin ok!\r\n");
            str = "EA101EB";
        } else {
            printf("Reg admin failed!\r\n");
            str = "EA100EB";
        }

    } else if (strncmp(msg[0], "11", strlen("11")) ==0 ) {
        printf("Reg teacher\r\n");
        ret = reg_teacher(base64_decode(msg[1], strlen(msg[1])), msg[2], msg[3]);
        if (ret == 0) {
            printf("Reg teacher ok!\r\n");
            str = "EA111EB";
        } else {
            str = "EA110EB";
        }
    } else if (strncmp(msg[0], "12", strlen("12")) == 0){
        printf("Reg student\r\n");
        ret = reg_stu_to_db(base64_decode(msg[1], strlen(msg[1])), msg[2], msg[3]); 
        if (ret == 0) {
            printf("Reg student ok!\r\n");
            str = "EA121EB";
        } else {
            str = "EA120EB";
        }
    } else if (strncmp(msg[0], "20", strlen("20")) == 0) {
        printf("Find admin message!\r\n");
    } else if (strncmp(msg[0], "21", strlen("21")) == 0) {
        printf("Find teacher message!\r\n");
    } else if (strncmp(msg[0], "22", strlen("22")) == 0) {
        printf("Find students message!\r\n");
        char *msg_buf = malloc(sizeof(char) * 64);
        memset(msg_buf, 0, 64);
        find_stu_info(msg[1], msg_buf, 64);
        snprintf(pkt_buf, sizeof(pkt_buf), "EA221%s|EB", msg_buf);
        str = pkt_buf;
        free(msg_buf);
    } else if (strncmp(msg[0], "32", strlen("32")) == 0) {
        ret = update_stu_passwd(msg[1], msg[2], strlen(msg[2]));
        if (ret == 0) {
            str = "EA321EB";
        } else {
            str = "EA320EB";
        }
    } else if (strncmp(msg[0], "31", strlen("31")) == 0) {
        ret = update_teacher_passwd(msg[1], msg[2], strlen(msg[2]));
        if (ret == 0) {
            str = "EA311EB";
        } else {
            str = "EA310EB";
        }
    } else if (strncmp(msg[0], "30", strlen("30")) == 0) {
        ret = update_admin_passwd(msg[1], msg[2], strlen(msg[2]));
        if (ret == 0) {
            str = "EA301EB";
        } else {
            str = "EA300EB";
        }
    } else if (strncmp(msg[0], "40", strlen("40")) == 0) {
        send_teacher_msg_to_client(client->fd);
        send_student_msg_to_client(client->fd);
        //send_exam_msg_to_admin
        send_exam_msg_to_admin(client->fd);
    } else if (strncmp(msg[0], "42", strlen("42")) == 0) {
        printf("Send %s msg to client\r\n", msg[1]);
        send_examed_to_student(client->fd, msg[1], strlen(msg[1]));
        send_examing_to_student(client->fd, msg[1], strlen(msg[1]));
    } else if (strncmp(msg[0], "41", strlen("41")) == 0) {
        send_examed_to_teacher(client->fd, msg[1], strlen(msg[1]));
        send_examing_to_teacher(client->fd, msg[1], strlen(msg[1]));
        send_questiondb_to_client(client->fd, msg[1]);
    } else if (strncmp(msg[0], "43", strlen("43")) == 0) {
        printf("Replace major, update question\r\n");
        send_questiondb_to_client(client->fd, msg[1]);
    } else if (strncmp(msg[0], "52", strlen("52")) == 0) {
        printf("Send paper to client\r\n");
        send_paper_to_student(client->fd, msg[1], strlen(msg[1]), msg[2], strlen(msg[2]));

    } else if (strncmp(msg[0], "50", strlen("50")) == 0) {
        ret = add_exam_to_db(msg[1], base64_decode(msg[2], strlen(msg[2])), msg[3], msg[4], msg[5], msg[6]);
        if (ret == 0) {
            str = "EA501EB";
        } else {
            str = "EA500EB";
        }
        printf("add exam to db\r\n");
    } else if (strncmp(msg[0], "53", strlen("53")) == 0) {
        printf("Del exam\r\n");
        ret = del_exam(msg[1]);
        if (ret == 0) {
            str = "EA501EB";
        } else {
            str = "EA500EB";
        }
    } else if (strncmp(msg[0], "54", strlen("54")) == 0) {
        ret = del_teacher(msg[1]);
        if (ret == 0) {
            str = "EA501EB";
        } else {
            str = "EA500EB";
        }

    } else if (strncmp(msg[0], "55", strlen("55")) == 0) {
        ret = del_student(msg[1]);
        if (ret == 0) {
            str = "EA501EB";
        } else {
            str = "EA500EB";
        }

    } else if (strncmp(msg[0], "62", strlen("62")) == 0) {
        printf("Search Paper\r\n");
        send_paper_to_student(client->fd, msg[1], strlen(msg[1]), msg[2], strlen(msg[2]));
        str = "EA623EB";
    } else if (strncmp(msg[0], "61", strlen("61")) == 0) {
        printf("Add question to db\r\n");
        ret = add_question_to_db(msg[1], base64_decode(msg[2], strlen(msg[2])), \
                                 base64_decode(msg[3], strlen(msg[3])), \
                                 base64_decode(msg[4], strlen(msg[4])));
        if (ret == 0) {
            str = "EA611EB";
        } else {
            str = "EA610EB";
        }
    } else if (strncmp(msg[0], "72", strlen("72")) == 0) {
        printf("Insert user question to db\r\n");
        add_questioned_to_db(msg[1], base64_decode(msg[2], strlen(msg[2])), msg[3], \
                             base64_decode(msg[4], strlen(msg[4])), msg[5], msg[6]);
    } else if (strncmp(msg[0], "82", strlen("82")) == 0) {
        ret = add_score_to_examed(msg[1], msg[2], base64_decode(msg[3], strlen(msg[3])), msg[4]);
        if (ret == 0) {
            str="EA821EB";
        }
    } else if (strncmp(msg[0], "92", strlen("92")) == 0) {
        printf("Genrate paper\r\n");
        ret = send_paper_to_student();
    }

    if (str != NULL) {
        memcpy(sendbuf, str, strlen(str));
        send(client->fd, sendbuf, strlen(sendbuf), 0); 
        printf("[%d]Back Packet: %s\r\n", client->fd, sendbuf);
    }
    
}  
