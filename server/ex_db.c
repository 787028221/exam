#include <mysql/mysql.h>
#if defined(_WIN32) || defined(_WIN64) //Ϊ��֧��windowsƽ̨�ϵı���
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>   
#include <unistd.h>   
#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <arpa/inet.h>   
#include <sys/time.h>   
#include <stdlib.h>
#include "ex_db.h"
#include "mysql/mysql.h"  //�ҵĻ����ϸ��ļ���/usr/local/include/mysql��

#define db_log printf

typedef struct _server_ctx {
    MYSQL connection;

} ex_server_ctx;

static int server_stat = -1;

static ex_server_ctx serverctx;


int init_db()
{
    int ret;
    char sql_str[256];


    mysql_init(&serverctx.connection);
    if (!mysql_real_connect(&serverctx.connection, "localhost",
                "root", "123456", "examonline", 0, NULL, 0)) {
        db_log("Connet database err!\r\n");
        ret = DB_CON_ERR;
        goto TAR_OUT;
    }

    snprintf(sql_str, sizeof(sql_str), \
         "CREATE TABLE teachers(name varchar(64), teacherid varchar(32), passwd varchar(64)");
    mysql_query(&serverctx.connection, sql_str);
    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE admin(name varchar(64), adminid varchar(32), passwd varchar(64))");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE students(name varchar(64), stuid varchar(32), passwd varchar(64)");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE scores(name varchar(64), stuid varchar(32), major varchar(64), score varchar(8)");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE examedPaper(stuid varchar(32), paperid varchar(64), major varchar(64), score varchar(8), fullscore varchar(8))");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE examed(teacherid varchar(32), stuid varchar(32)，paperid varchar(64), major varchar(64), score varchar(8), fullscore varchar(8), diffcult varchar(8))");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE examing(teacherid varchar(32), stuid varchar(32), paperid varchar(64), major varchar(64), fullscore varchar(8), diffcult varchar(8), time varchar(32))");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE examingPaper(paperId varchar(32), major varchar(64), TotalScore varchar(4), examTime varchar(8))");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE paper(paperid varchar(64), major varchar(64), members varchar(8), diffcult varchar(8))");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE question(paperid varchar(64), major varchar(64), "
             "type varchar(4), diffcult varchar(4), question varchar(2048), answer varchar(128))");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE stu_paper(paperid varchar(64), stuid varchar(64))");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE teacher_paper(paperid varchar(64), teacherid varchar(64))");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE questiondb(questiontype varchar(8), major varchar(64), question varchar(4096), choice varchar(8), answer varchar(128))");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE score(stuid varchar(64), major varchar(64), paperid varchar(64), score varchar(4))");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE score(stuid varchar(64), major varchar(64), paperid varchar(64), score varchar(4))");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE questioned(stuid varchar(64), major varchar(64), "
             "paperid varchar(64), question varchar(4096), answer varchar(32), myanswer varchar(32))");
    mysql_query(&serverctx.connection, sql_str);

    snprintf(sql_str, sizeof(sql_str), \
         "CREATE TABLE paperdb(question_id Int, question_type varchar(4), major varchar(64), "
         "question varchar(4096), answer varchar(128)");
    mysql_query(&serverctx.connection, sql_str);
   // snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE questiondb(questiontype varchar(8), major varchar(64), question varchar(4096), option varchar(8), answer varchar(128))");
   // mysql_query(&serverctx.connection, sql_str);
    init_dbsock();
    ret = 0;
    server_stat = 1;

     
TAR_OUT:
    return ret;
}

int close_db()
{
    int ret;

    mysql_close(&serverctx.connection);
    
    return ret;
}

int reg_stu_to_db(char *name, char *stu_id, char *passwd)
{
    int ret = -1;
    char sql_str[256];

    snprintf(sql_str, sizeof(sql_str), "INSERT INTO students(name, stuid, passwd) VALUES('%s', '%s', '%s')", name, stu_id, passwd);
    if (server_stat) {
        ret = mysql_query(&serverctx.connection, sql_str);
        if (ret) {
            goto TAR_OUT;
        }
        ret = 0;
    }

TAR_OUT:
    return ret;
}

int reg_admin(char *name, char *id, char *passwd)
{
    int ret = -1;
    char sql_str[256];

    if (!server_stat) {
        ret = -1;
        goto TAR_OUT;
    }
    snprintf(sql_str, sizeof(sql_str), "INSERT INTO admin(name, adminid, passwd) VALUES('%s', '%s', '%s')", name, id, passwd);

    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        goto TAR_OUT;
    }
    ret = 0;


TAR_OUT:
    return ret;
}



int reg_teacher(char *name, char *id, char *passwd)
{
    int ret = -1;
    char sql_str[256];
    snprintf(sql_str, sizeof(sql_str), "INSERT INTO teachers(name, teacherid, passwd) VALUES('%s', '%s', '%s')", name, id, passwd);
    if (server_stat) {
        ret = mysql_query(&serverctx.connection, sql_str);
        if (ret) {
            goto TAR_OUT;
        }
        ret = 0;
    }

TAR_OUT:
    return ret;
}

int serach_stu(char *stu_id, char *passwd)
{
    int ret = -1;
    int i;
    char sql_str[256];
    // mysql结果集  
    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat) {
        ret = -3;
        goto TAR_OUT;
    }

    printf("stu_id %s passwd %s\r\n", stu_id, passwd);
    snprintf(sql_str, sizeof(sql_str), "SELECT passwd  from students where stuid=%s", stu_id);

    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        goto TAR_OUT;
    }
 

    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Store result err!");
        ret = -1;
        goto TAR_OUT;
    }

    while (mysql_row = mysql_fetch_row(mysql_res)) {
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
            if (strlen(mysql_row[i]) == strlen(passwd) && \
                strncmp(mysql_row[i], passwd, strlen(passwd)) == 0) {
                printf("check ok!\r\n");
                ret = 0;
                goto TAR_OUT; 
            } else {
                ret = -1;
                goto TAR_OUT;
            }
        }
    }

    ret = -2;
TAR_OUT:

    return ret;
}


int serach_teacher(char *teacher_id, char *passwd)
{
    int ret = -1;
    int i;
    char sql_str[256];
    // mysql结果集  
    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    printf("teacher_id %s passwd %s\r\n", teacher_id, passwd);
    snprintf(sql_str, sizeof(sql_str), "SELECT passwd  from teachers where teacherid=%s", teacher_id);
    if (server_stat) {
        ret = mysql_query(&serverctx.connection, sql_str);
        if (ret) {
            goto TAR_OUT;
        }
        ret = 0;
    }

    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        ret = -1;
        goto TAR_OUT;
    }

    while (mysql_row = mysql_fetch_row(mysql_res)) {
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
            if (strlen(mysql_row[i]) == strlen(passwd) && \
                strncmp(mysql_row[i], passwd, strlen(passwd)) == 0) {
                printf("check ok!\r\n");
                ret = 0;
                goto TAR_OUT; 
            } else {
                ret = -1;
                goto TAR_OUT;
            }
        }
    }
    ret = -2;
TAR_OUT:

    return ret;
}

int serach_admin(char *id, char *passwd)
{
    int ret = -1;
    int i;
    char sql_str[256];
    // mysql结果集  
    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat) {
        printf("Program stat err!\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    printf("admin %s passwd %s\r\n", id, passwd);
    snprintf(sql_str, sizeof(sql_str), "SELECT passwd FROM admin WHERE adminid=%s", id);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("SQL Query err!\r\n");
        goto TAR_OUT;
    }
    ret = -1;

    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }

    printf("Seach each other!\r\n");
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        ret = -2;
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
            if (strlen(mysql_row[i]) == strlen(passwd) && \
                strncmp(mysql_row[i], passwd, strlen(passwd)) == 0) {
                printf("check ok!\r\n");
                ret = 0;
                goto TAR_OUT; 
            } else {
                ret = -1;
            }
        }
    }

TAR_OUT:

    return ret;
}

int find_stu_info(char *stu_id, char *str, int len)
{
    int ret = -1;
    int i;
    char *p_str = NULL;
    char sql_str[256];
    char str_tmp[2][64] = {0, 0};
    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat || str == NULL || len == 0) {
        printf("Paremeter is illgal[%s][%s][%d]!\r\n", stu_id, str, len);
        ret = -1;
        goto TAR_OUT;
    }
    printf("stu_id %s\r\n", stu_id);
    snprintf(sql_str, sizeof(sql_str), "SELECT name, stuid  FROM students where stuid=%s", stu_id);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    int flag = 0;
    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    printf("Searching....\r\n");
    int row = 0;
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        printf("Fetch...\r\n");
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
            if (i < 2) {
                flag = 1;
                strcpy(str_tmp[i], mysql_row[i]);
            }
        }
        row++;
        ret = -1;
    }

    if (flag) {
        printf("str_tmp : [%s][%s]\r\n", str_tmp[0], str_tmp[1]);
        snprintf(str, len, "%s|%s", str_tmp[0], str_tmp[1]);
    }
TAR_OUT:
    printf("Result str: %s\r\n", str);
    return ret;
}

int update_stu_passwd(char *stu_id, char *passwd, int passwd_len)
{
    int ret;
    char sql_str[256];

    if (!server_stat || passwd == NULL || passwd_len == 0) {
        printf("Paremeter is illgal[%s][%s][%d]!\r\n", stu_id, passwd, passwd_len);
        ret = -1;
        goto TAR_OUT;
    }
    printf("stu_id %s update password\r\n", stu_id);
    snprintf(sql_str, sizeof(sql_str), "UPDATE students SET passwd=%s WHERE stuid=%s", passwd, stu_id);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

TAR_OUT:

    return ret;
}

int update_stu_name(char *stu_id, char *name)
{
    int ret;

    return ret;
}

int update_teacher_passwd(char *teacher_id, char *passwd, int passwd_len)
{
    int ret;
    char sql_str[256];

    if (!server_stat || passwd == NULL || passwd_len == 0) {
        printf("Paremeter is illgal[%s][%s][%d]!\r\n", teacher_id, passwd, passwd_len);
        ret = -1;
        goto TAR_OUT;
    }
    printf("teacher_id %s update password\r\n", teacher_id);
    snprintf(sql_str, sizeof(sql_str), "UPDATE teachers SET passwd=%s WHERE teacherid=%s", passwd, teacher_id);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }
TAR_OUT:
    return ret;
}

int update_teacher_name(char *teacher_id, char *name, int len)
{
    int ret;

    return ret;
}

int update_admin_passwd(char *teacher_id, char *passwd, int len)
{
    int ret;

    return ret;
}

int update_admin_name(char *teacher_id, char *name, int len)
{
    int ret;

    return ret;
}

int send_teacher_msg_to_client(int fd)
{
    int ret = -1;
    int i;
    char *p_str = NULL;
    char sql_str[256];
    char str_tmp[2][64] = {0, 0};
    char str[128] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat || fd < 0) {
        printf("Paremeter is illgal[%d]!\r\n",  fd);
        ret = -1;
        goto TAR_OUT;
    }
   
    snprintf(sql_str, sizeof(sql_str), "SELECT name, teacherid  FROM teachers");
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    int flag = 0;
    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    printf("Sending....\r\n");
    int row = 0;
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        printf("Fetch...\r\n");
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
            if (i < 2) {
                flag = 1;
                strcpy(str_tmp[i], mysql_row[i]);
            }
        }

        if (flag) {
            snprintf(str, sizeof(str), "EA401%d|%s|%s|EB", row, str_tmp[0], str_tmp[1]);
            printf("Send to Client str : [%s]\r\n", str);
            send(fd, str, strlen(str), 0); 
            flag = 0;
        }
        row++;
    }

TAR_OUT:
    printf("Result str: %s\r\n", str);

    return ret;
}

int send_student_msg_to_client(int fd)
{
    int ret = -1;
    int i;
    char *p_str = NULL;
    char sql_str[256];
    char str_tmp[2][64] = {0, 0};
    char str[128] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat || fd < 0) {
        printf("Paremeter is illgal[%d]!\r\n",  fd);
        ret = -1;
        goto TAR_OUT;
    }
   
    snprintf(sql_str, sizeof(sql_str), "SELECT name, stuid  FROM students");
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    int flag = 0;
    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    printf("Sending....\r\n");
    int row = 0;
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        printf("Fetch...\r\n");
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
            if (i < 2) {
                flag = 1;
                strcpy(str_tmp[i], mysql_row[i]);
            }
        }

        if (flag) {
            snprintf(str, sizeof(str), "EA402%d|%s|%s|EB\0", row, str_tmp[0], str_tmp[1]);
            printf("Send to Client str : [%s]\r\n", str);
            send(fd, str, strlen(str), 0); 
            flag = 0;
        }
        row++;
    }

TAR_OUT:

    return ret;
}

int send_examing_to_admin()
{ 
    int ret;

    return ret;
}

int send_exam_msg_to_admin(int fd)
{
    int ret = -1;
    int i;
    char *p_str = NULL;
    char sql_str[256];
    char str_tmp[4][64] = {0, 0, 0, 0};
    char str[128] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat || fd < 0) {
        printf("Paremeter is illgal[%d]!\r\n",  fd);
        ret = -1;
        goto TAR_OUT;
    }
   
    snprintf(sql_str, sizeof(sql_str), "SELECT paperid, major FROM examing"); 
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    int flag = 0;
    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    printf("Sending....\r\n");
    int row = 0;
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        printf("Fetch...\r\n");
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
                flag = 1;
                strcpy(str_tmp[i], mysql_row[i]);
        }

        if (flag) {
            snprintf(str, sizeof(str), "EA403%d|%s|%s|EB", row, str_tmp[0], str_tmp[1]);
            printf("Send to Client str : [%s]\r\n", str);
            send(fd, str, strlen(str), 0); 
            flag = 0;
        }
        row++;
    }

TAR_OUT:

    return ret;
}

int add_exam_to_db(char *paperid, char *major, char *fullscore, char *teacherid, char *stuid, char *time)
{
    int ret = -1;
    char sql_str[256];
    snprintf(sql_str, sizeof(sql_str), "INSERT INTO examing(paperid, major, fullscore, teacherid, stuid, time) "
                                       "VALUES('%s', '%s', '%s', '%s', '%s', '%s')", paperid, major, fullscore, \
             teacherid, stuid, time);
    if (server_stat) {
        ret = mysql_query(&serverctx.connection, sql_str);
        if (ret) {
            goto TAR_OUT;
        }
        ret = 0;
    }

TAR_OUT:
    return ret;
}

void del_exam(char *paperid)
{
    int ret;
    char sql_str[256];

    if (!server_stat || paperid == NULL) {
        printf("Paremeter is illgal[%s][%s][%d]!\r\n", paperid);
        ret = -1;
        goto TAR_OUT;
    }
    printf("Del exam [%s] \r\n", paperid);
    snprintf(sql_str, sizeof(sql_str), "DELETE FROM examing WHERE paperid='%s'", paperid);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        ret = -1;
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    snprintf(sql_str, sizeof(sql_str), "DELETE FROM examingPaper WHERE paperId='%s'", paperid);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        ret = -1;
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }
    ret = 0;
TAR_OUT:
    return ret;
}

int send_examing_to_student(int fd, char *stuid, int len)
{
    int ret = -1;
    int i;
    char *p_str = NULL;
    char sql_str[256];
    char str_tmp[4][64] = {0, 0, 0, 0};
    char str[128] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat || len <= 0) {
        printf("Paremeter is illgal[%d]!\r\n",  len);
        ret = -1;
        goto TAR_OUT;
    }
   
    snprintf(sql_str, sizeof(sql_str), "SELECT paperId, major, TotalScore, examTime FROM examingPaper"); 
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    int flag = 0;
    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    printf("Sending....\r\n");
    int row = 0;
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        printf("Fetch...\r\n");
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
                flag = 1;
                strcpy(str_tmp[i], mysql_row[i]);
        }

        if (flag) {
            snprintf(str, sizeof(str), "EA421%d|%s|%s|%s|%s|EB\0", row, str_tmp[0], str_tmp[1], str_tmp[2], str_tmp[3]);
            printf("Send to Client str : [%s]\r\n", str);
            send(fd, str, strlen(str), 0); 
            flag = 0;
        }
        row++;
    }

TAR_OUT:

    return ret;
}

/*
*将已经考过试的科目包括成绩发送到学生客户端
*/
int send_examed_to_student(int fd, char *stu_id, int len)
{
    int ret = -1;
    int i;
    char *p_str = NULL;
    char sql_str[256];
    char str_tmp[4][64] = {0, 0, 0, 0};
    char str[128] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat || fd < 0 || len <= 0) {
        printf("Paremeter is illgal[%d]!\r\n",  fd);
        ret = -1;
        goto TAR_OUT;
    }
   
    snprintf(sql_str, sizeof(sql_str), "SELECT paperid, major, score, fullscore FROM examedPaper WHERE stuid=%s", stu_id); 
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    int flag = 0;
    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    printf("Sending....\r\n");
    int row = 0;
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        printf("Fetch...\r\n");
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
                flag = 1;
                strcpy(str_tmp[i], mysql_row[i]);
        }

        if (flag) {
            snprintf(str, sizeof(str), "EA422%d|%s|%s|%s|%s|EB\0", row, str_tmp[0], str_tmp[1], str_tmp[2], str_tmp[3]);
            printf("Send to Client str : [%s]\r\n", str);
            send(fd, str, strlen(str), 0); 
            flag = 0;
        }
        row++;
    }

TAR_OUT:

    return ret;
}

/*
*将已经考过试的科目包括成绩发送到教师客户端
*/
int send_examed_to_teacher(int fd, char *teacherid, int len)
{
    int ret = -1;
    int i;
    char *p_str = NULL;
    char sql_str[256];
    char str_tmp[5][64] = {0, 0, 0, 0};
    char str[128] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat || fd < 0) {
        printf("Paremeter is illgal[%d]!\r\n",  fd);
        ret = -1;
        goto TAR_OUT;
    }
   
    snprintf(sql_str, sizeof(sql_str), "SELECT paperid, major, score, fullscore, students.name FROM examedPaper "
                                       "INNER JOIN students ON students.stuid=examedPaper.stuid"); 
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    int flag = 0;
    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    printf("Sending....\r\n");
    int row = 0;
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        printf("Fetch...\r\n");
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
                flag = 1;
                strcpy(str_tmp[i], mysql_row[i]);
        }

        if (flag) {
            snprintf(str, sizeof(str), "EA411%d|%s|%s|%s|%s|%s|EB\0", \
                     row, str_tmp[0], str_tmp[1], str_tmp[2], str_tmp[3], str_tmp[4]);
            printf("Send to Client str : [%s]\r\n", str);
            send(fd, str, strlen(str), 0); 
            flag = 0;
        }
        row++;
    }

TAR_OUT:

    return ret;
}

/*
*将正要考试的科目发送到教师客户端
*/
int send_examing_to_teacher(int fd, char *teacherid, int len)
{
    int ret = -1;
    int i;
    char *p_str = NULL;
    char sql_str[256];
    char str_tmp[5][64] = {0, 0, 0, 0};
    char str[128] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat || fd < 0) {
        printf("Paremeter is illgal[%d]!\r\n",  fd);
        ret = -1;
        goto TAR_OUT;
    }
   
    snprintf(sql_str, sizeof(sql_str), "SELECT paperid, major  from examing"
                                       " WHERE teacherid=%s", teacherid); 
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    int flag = 0;
    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    printf("Sending examing....\r\n");
    int row = 0;
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        printf("Fetch...\r\n");
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
                flag = 1;
                strcpy(str_tmp[i], mysql_row[i]);
        }

        if (flag) {
            snprintf(str, sizeof(str), "EA412%d|%s|%s|EB\0", row, str_tmp[0], str_tmp[1]);
            printf("Send to Client str : [%s]\r\n", str);
            send(fd, str, strlen(str), 0); 
            flag = 0;
        }
        row++;
    }

TAR_OUT:

    return ret;
}

int send_questiondb_to_client(int fd, char *major)
{
    int ret = -1;
    int i;
    char *p_str = NULL;
    char sql_str[256];
    char str_tmp[5][4096] = {0, 0, 0, 0};
    char str[4096] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat || fd < 0) {
        printf("Paremeter is illgal[%d]!\r\n",  fd);
        ret = -1;
        goto TAR_OUT;
    }
   
    snprintf(sql_str, sizeof(sql_str), "SELECT questionId, questiontype, question, answer  from questiondb"); 
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    int flag = 0;
    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    printf("[%s]Sending examing....\r\n", __func__);
    int row = 0;
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        printf("Fetch...\r\n");
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
            flag = 1;
            if (mysql_row[i] == NULL) {
                continue;
            }
            strcpy(str_tmp[i], mysql_row[i]);
        }

        if (flag) {
            snprintf(str, sizeof(str), "EA413%d|%s|%s|%s|%s|EB", row, str_tmp[0], \
                     str_tmp[1], base64_encode(str_tmp[2], strlen(str_tmp[2])), \
                     base64_encode(str_tmp[3], strlen(str_tmp[3])));
            printf("Send to Client str : [%s]\r\n", str);
            send(fd, str, strlen(str), 0); 
            flag = 0;
        }
        row++;
    }

TAR_OUT:

    return ret;
}

/*
*将试题添加到试卷上
*/
int add_to_paper(char *paperid,  char *major, char *type, \
                          char *question, char *answer)
{
    int ret = -1;
    char sql_str[256];
    snprintf(sql_str, sizeof(sql_str), "INSERT INTO question(paperid, major, type, question, answer) VALUES('%s', '%s', '%s', '%s'，‘%s’)", \
             paperid, major, type, question, answer);
    if (server_stat) {
        ret = mysql_query(&serverctx.connection, sql_str);
        if (ret) {
            goto TAR_OUT;
        }
        ret = 0;
    }

TAR_OUT:
    return ret;
}

/*标记学生*/
int add_to_paper_stu(char *paperid, char *stuid)
{
    int ret = -1;
    char sql_str[256];
    snprintf(sql_str, sizeof(sql_str), "INSERT INTO stu_paper(paperid, stuid) VALUES('%s', '%s')", \
             paperid, stuid);
    if (server_stat) {
        ret = mysql_query(&serverctx.connection, sql_str);
        if (ret) {
            goto TAR_OUT;
        }
        ret = 0;
    }

TAR_OUT:
    return ret;
}

int add_to_paper_teacher(char *paperid, char *teacherid)
{
    int ret = -1;
    char sql_str[256];
    snprintf(sql_str, sizeof(sql_str), "INSERT INTO teacher_paper(paperid, teacherid) VALUES('%s', '%s')", \
             paperid, teacherid);
    if (server_stat) {
        ret = mysql_query(&serverctx.connection, sql_str);
        if (ret) {
            goto TAR_OUT;
        }
        ret = 0;
    }

TAR_OUT:
    return ret;
}

int add_score_to_db(char *paperid, char *stuid, char *major, char *score)
{
    int ret = -1;
    char sql_str[256];

    if (!paperid || !stuid || !major || !score) {
        ret = -2;
        goto TAR_OUT;
    }
    snprintf(sql_str, sizeof(sql_str), "INSERT INTO score(paperid, stuid, major, score) VALUES('%s', '%s', '%s', '%s')",\
             paperid, stuid, major, score);
    if (server_stat) {
        ret = mysql_query(&serverctx.connection, sql_str);
        if (ret) {
            goto TAR_OUT;
        }
        ret = 0;
    }

TAR_OUT:
    return ret;
}

int send_score_to_teacher(int fd, char *teacherid, int teacherid_len)
{
    int ret = -1;
    int i;
    char *p_str = NULL;
    char sql_str[256];
    char str_tmp[4][128] = {0, 0, 0, 0};
    char str[128] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat || fd < 0 || !teacherid) {
        printf("Paremeter is illgal[%d]!\r\n",  fd);
        ret = -1;
        goto TAR_OUT;
    }
    snprintf(sql_str, sizeof(sql_str), "SELECT paperid, stuid, major, score from score");
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    int flag = 0;
    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    printf("Sending....\r\n");
    int row = 0;
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        printf("Fetch...\r\n");
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
                flag = 1;
                strcpy(str_tmp[i], mysql_row[i]);
        }

        if (flag) {
            snprintf(str, sizeof(str), "EA412%d|%s|%s|%s|%s|EB\0", row, str_tmp[0], str_tmp[1], str_tmp[2], str_tmp[3]);
            printf("Send to Client str : [%s]\r\n", str);
            send(fd, str, strlen(str), 0); 
            flag = 0;
        }
        row++;
    }

TAR_OUT:
    return ret;
}

int insert_to_paperdb(int question_id, char *question_type, char *major, char *question, char *answer)
{
    int ret = -1;
    char sql_str[128];

    snprintf(sql_str, sizeof(sql_str), \
     "INSERT INTO paperdb(question_id, question_type, major, question, answer) VALUES(%d, '%s', '%s', '%s', '%s')",  \
             question_id, question_type, major, question, answer);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

TAR_OUT:
    return ret;

}
int send_paper_to_student(int fd, char *stuid, int stuid_len, char *paperid, int paperid_len)
{
    int ret = -1;
    int i;
    char *p_str = NULL;
    char sql_str[256];
    char str_tmp[4][4096] = {0, 0, 0, 0};
    char str[4096] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat || fd < 0) {
        printf("Paremeter is illgal[%d]!\r\n",  fd);
        ret = -1;
        goto TAR_OUT;
    }
   
    snprintf(sql_str, sizeof(sql_str), "SELECT quesType, question, answer FROM p%s", paperid);
    printf("sql_str: %s\r\n", sql_str);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    int flag = 0;
    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    printf("Sending....\r\n");
    int row = 0;
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        printf("Fetch...\r\n");
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
                flag = 1;
                strcpy(str_tmp[i], mysql_row[i]);
        }

        if (flag) {
            snprintf(str, sizeof(str), "EA621%d|%s|%s|%s|%s|EB\0", row, str_tmp[0], str_tmp[1], str_tmp[2], str_tmp[3]);
            printf("Send to Client str : [%s]\r\n", str);
            send(fd, str, strlen(str), 0); 
            flag = 0;
        }
        row++;
    }

TAR_OUT:

    return ret;
}

int serach_paper(int fd, char *stuid, int stuid_len, char *paperid, int paperid_len)
{
    int ret = -1;
    int i;
    char *p_str = NULL;
    char sql_str[256];
    char str_tmp[4][1024] = {0, 0, 0, 0};
    char str[2048] = {0};
    //char question[10240] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    if (!server_stat || fd < 0) {
        printf("Paremeter is illgal[%d]!\r\n",  fd);
        ret = -1;
        goto TAR_OUT;
    }
   
    snprintf(sql_str, sizeof(sql_str), "SELECT type, question, answer FROM question WHERE paperid=%s", paperid); 
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    int flag = 0;
    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    printf("Sending....\r\n");
    int row = 0;
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        printf("Fetch...\r\n");
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
                flag = 1;
                strcpy(str_tmp[i], mysql_row[i]);
        }

        if (flag) {
            snprintf(str, sizeof(str), "EA521%d|%s|%s|%s|%s|EB\0", row, str_tmp[0], str_tmp[1], str_tmp[2], str_tmp[3]);
            printf("Send to Client str : [%s]\r\n", str);
            send(fd, str, strlen(str), 0); 
            flag = 0;
        }
        row++;
    }

TAR_OUT:

    return ret;
}
/*CREATE TABLE questiondb(questiontype varchar(8), 
  major varchar(64), question varchar(4096), choice varchar(8), answer varchar(128))*/

int add_question_to_db(char *questionId, char *questiontype, char *major, char *question, char *answer)
{
    int ret = -1;
    char sql_str[4096];

    if (!server_stat) {
        return DB_STAT_ERR;
    }
    snprintf(sql_str, sizeof(sql_str), "INSERT INTO questiondb(questionId, questiontype, major, question, answer) "
                                       "VALUES('%s', '%s', '%s', '%s', '%s')", questionId, questiontype, major, question, answer);
    printf("sql_str: %s\r\n", sql_str);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        goto TAR_OUT;
    }

TAR_OUT:

    return ret;
}

int del_question_from_db(char *questionId)
{
    int ret;
    char sql_str[512];

    snprintf(sql_str, sizeof(sql_str), "DELETE FROM questiondb WHERE questionId='%s'", questionId);
    if (server_stat) {
        ret = mysql_query(&serverctx.connection, sql_str);
        if (ret) {
            goto TAR_OUT;
        }
        ret = 0;
    }

TAR_OUT:
    return ret;
}

int create_questioned_db(char *stuId, char *paperId)
{
    int ret;
    char *sql_str;

    sql_str = (char *)malloc(sizeof(char) * 4096);
    if (!sql_str) {
        printf("Malloc err\r\n");
        ret = DB_MEM_ERR;
        goto TAR_OUT;
    }

    snprintf(sql_str, 4096, \
         "CREATE TABLE qed%s%s(questionId varchar(64), questiontype varchar(8), question varchar(4096), "
         "answer varchar(128), myanswer varchar(128))", \
             stuId, paperId);
    printf("sql_str : %s\r\n", sql_str);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Sql create questioned db err!\r\n");
        goto TAR_OUT;
    }

TAR_OUT:

    if (sql_str) {
        free(sql_str);
    }
    return ret;
}

int add_questioned_to_db(char *paperid, char *stuid, char *questionId, char *questiontype, \
                          char *question, char *answer, char *myanswer)
{
    int ret = -1;
    char sql_str[4096];

    if (server_stat) {

    }
    snprintf(sql_str, sizeof(sql_str), "INSERT INTO qed%s%s(questionId, questiontype, question, answer, myanswer) "
                                       "VALUES('%s', '%s', '%s', '%s', '%s')", stuid, paperid, questionId, questiontype, \
             question, answer, myanswer);
    printf("sql_str: %s\r\n", sql_str);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Sql INSERT INTO  qeddb err![%s]\r\n", mysql_error(&serverctx.connection));
        goto TAR_OUT;
    }

TAR_OUT:

    return ret;
}


int add_score_to_examed(char *stuid, char *paperid, char *major, char *score, char *fullscore)
{
    int ret = -1;
    char sql_str[4096];
    char teacherid[64] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    #if 0
    snprintf(sql_str, sizeof(sql_str), "SELECT teacherid form examing WHERE paperid=%s", paperid);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    int flag = 0;
    mysql_res = mysql_store_result(&serverctx.connection);
    if (!mysql_res) {
        printf("Search result is null\r\n");
        ret = -1;
        goto TAR_OUT;
    }
    int row = 0;
    int i;
    while (mysql_row = mysql_fetch_row(mysql_res)) {
        printf("Fetch...\r\n");
        for (i=0; i<mysql_num_fields(mysql_res); i++) {
            printf("mysql search : %s\r\n", mysql_row[i]);
                flag = 1;
                strcpy(teacherid[i], mysql_row[i]);
                break;
        }
        if (flag) {
            break;
        }

    }
#endif
    snprintf(sql_str, sizeof(sql_str), "INSERT INTO examedPaper(stuid, paperid, major, score, fullscore) "
                                       "VALUES('%s', '%s', '%s', '%s', '%s')", \
             stuid, paperid, major, score, fullscore);
    if (server_stat) {
        ret = mysql_query(&serverctx.connection, sql_str);
        if (ret) {
            printf("Sql INSERT INTO examedPaper err\r\n");
            goto TAR_OUT;
        }
        ret = 0;
    }

    snprintf(sql_str, sizeof(sql_str), "DELETE FROM  examingPaper WHERE paperid='%s'", paperid);
    printf("sql str %s\r\n", sql_str);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("Sql DELETE FROM  examingPaper err\r\n");
        goto TAR_OUT;
    } else {
        printf("Delete paper\r\n");
    }
    ret = 0;

TAR_OUT:
    return ret;
}

int del_teacher(char *teacherid)
{
    int ret;
    char sql_str[4096];
    //char teacherid[64] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    snprintf(sql_str, sizeof(sql_str), "DELETE FROM teachers WHERE teacherid=%s", teacherid);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        ret = -3;
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    ret = 0;
TAR_OUT:
    return ret;
}

int del_student(char *stuid)
{
    int ret = -1;
    char sql_str[4096];
    char teacherid[64] = {0};

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    snprintf(sql_str, sizeof(sql_str), "DELETE FROM students WHERE stuid=%s", stuid);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        ret = -3;
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }

    ret = 0;

TAR_OUT:

    return ret;
}


int create_exam_paper(char *examid)
{
    int ret = -1;
    char sql_str[4096];

    printf("Create p%s table\r\n", examid);
    snprintf(sql_str, sizeof(sql_str), \
             "CREATE TABLE  IF NOT EXISTS p%s(questionId varchar(64), quesType varchar(8), "
             "question varchar(4096), answer varchar(128))", examid);
    printf("sql_str : %s\r\n", sql_str);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        ret = -3;
        printf("Query sql err!\r\n");
        goto TAR_OUT;
    }
    ret = 0;
TAR_OUT:

    return ret;
}


int add_question_in_paper(char *paperID, char *questionId, char *quesType, char *question, char *answer)
{
    int ret = -1;
    char *sql_str = NULL;

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    sql_str = (char *)malloc(sizeof(char) * 4096);
    if (!server_stat || sql_str == NULL) {
        ret = -4;
        goto TAR_OUT;
    }
    
    snprintf(sql_str, 4096, "INSERT INTO p%s(questionId, quesType, question, answer) VALUES('%s', '%s', '%s', '%s')", \
             paperID, questionId, quesType, question, answer);

    printf("Str sql: %s\r\n", sql_str);
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        printf("sql err!\r\n");
        goto TAR_OUT;
    }
    ret = 0;


TAR_OUT:
    if (sql_str) {
        free(sql_str);
    }
    return ret;
}

#define PAPER_PORT 4321
struct sockaddr_in ser_addr;  
struct sockaddr_in cli_addr;
int ser_sockfd,cli_sockfd;

int init_dbsock()
{
    int ret;
    int addlen;
    int err,n;  
    char temp[4][4096] = {0, 0, 0, 0};
    char *p_str = NULL;
    char *recvline;

    recvline = (char *)malloc (sizeof(char) * 4096);
    if (recvline == NULL) {
        ret = -4;
        goto TAR_OUT;
    }
    ser_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(ser_sockfd == -1) {  
        printf("socket error!\n");
        ret = -1;
        goto TAR_OUT;
    }
    bzero(&ser_addr, sizeof(ser_addr));  
    ser_addr.sin_family = AF_INET;  
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
    ser_addr.sin_port = htons(PAPER_PORT);  
    err = bind(ser_sockfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    if(err == -1) {  
        printf("bind error!\n");
        ret = -1;
        goto TAR_OUT;
    }

    err = listen(ser_sockfd, 5);  
    if(err==-1) {  
        printf("listen error\n");  
        ret = -1;
        goto TAR_OUT;
    }
    printf("listen the port: %d\n", PAPER_PORT);

TAR_OUT:
    if (recvline) {
        free(recvline);
    }
    return ret;
}

void recv_paper(void *arg)
{
    char *paperid;
    int err,n;  
    int addlen;
    int str_cnt = 0;
    int ret;
    char temp[5][8196] = {0, 0, 0, 0};
    char *p_str = NULL;
    char *recvline;
    char *questionId;
    char *quesType;
    char *question;
    char *answer;

    if (arg == NULL) {
        printf("paper id is null");
        return;
    }
    paperid = arg;

    recvline = (char *)malloc(sizeof(char) * 8196);
    ret = create_exam_paper(paperid);
    
    printf("listen the port: %d\n", PAPER_PORT);
    char *string = NULL;
    char *p;
    char *pdata;
    char strbase[2048] = {0};
    
    addlen = sizeof(struct sockaddr_in);
          
    while(1) {
        cli_sockfd = accept(ser_sockfd, (struct sockaddr *)&cli_addr, &addlen);
        if(cli_sockfd==-1) {
            printf("accept error\n");
        }
        printf("Accept client!\r\n");
        while(1) {
            printf("waiting for client...\n");
            n = recv(cli_sockfd, recvline, 8196, 0);
            if(n==-1) {
                printf("recv error\n");
            }
            string = recvline;
            printf("recvline: %s\r\n", recvline);

            p = strstr(string,  "EB");
            while (p != NULL) {
                memset(strbase, 0, sizeof(strbase));
                memset(temp, 0, sizeof(temp));
                printf("Find str:: %s[%p]\r\n", p, p);
                memcpy(strbase, string, p - string);
                strbase[p - string] = '\0';
                printf("Strbase : %s\r\n", strbase);
                pdata = &strbase[2];
                if (strcmp(pdata, "BYE") == 0) {
                    printf("Send over, Bye!\r\n");
                    goto TAR_OUT;
                }
                str_cnt = 0;
                while ((p_str = strsep(&pdata, "|")) != NULL) {
                    if (p_str != NULL) {
                        strcpy(temp[str_cnt], p_str);
                        //temp[str_cnt][strlen(p_str)] = '\0';
                        printf("Get item str: [%d] <%s>\r\n", str_cnt, p_str);
                        str_cnt++;
                        if (str_cnt >= 5) {
                            break;
                        }
                    }
                }
#if 0
                questionId = base64_decode(temp[1], strlen(temp[1])); 
                quesType = base64_decode(temp[2], strlen(temp[2])); 
                question = base64_decode(temp[3], strlen(temp[3])); 
                answer = base64_decode(temp[4], strlen(temp[4])); 
                if (!questionId || !quesType || !question || !answer) {
                    printf("Malloc err\r\n"); 
                    goto TAR_OUT;
                }
#endif             
               // ret = add_question_in_paper(paperid, questionId, quesType,\
                                          question , answer);                   
                ret = add_question_in_paper(paperid, temp[1], temp[2],\
                                          temp[3] , temp[4]);
#if 0
                free(questionId); 
                questionId = NULL; 
                free(quesType); 
                quesType = NULL; 
                free(question); 
                question = NULL; 
                free(answer); 
                answer = NULL; 
#endif                                
                string = &p[2];
                p = strstr(string,  "EB");
            }
            break;
        }
        close(cli_sockfd);  
    }  
      
TAR_OUT:
    if (recvline) {
        free(recvline);
    }
#if 0
if (questionId) {
    free(questionId);
}
if (quesType) {
    free(quesType);
}
if (question) {
    free(question);
}
if (answer) {
    free(answer);
}
#endif        
    printf("Exit thread\r\n");
    close(ser_sockfd);
    return;

}

/*paperId varchar(32), major varchar(64), TotalScore varchar(4), examTime varchar(8)*/
int add_examing_paper(char *paperId, char *major, char *TotalScore, char *examTime)
{
    int ret = -1;
    char *sql_str;

    MYSQL_RES *mysql_res;  
    // mysql行操作符  
    MYSQL_ROW mysql_row;

    sql_str = (char *)malloc(sizeof(char) * 4096);
    if (!sql_str) {
        printf("Malloc err!");
        ret = -4;
        goto TAR_OUT;
    }
    snprintf(sql_str, 4096, "INSERT INTO examingPaper(paperId, major, TotalScore, examTime) "
                                       "VALUES('%s', '%s', '%s', '%s')", paperId, major, TotalScore, examTime); 
    ret = mysql_query(&serverctx.connection, sql_str);
    if (ret) {
        ret = -3;
        printf("[%s]Query sql err!\r\n", __func__);
        goto TAR_OUT;
    }
    pthread_t id;
    ret = pthread_create(&id, NULL, (void  *)recv_paper, paperId);
    if (ret == 0) {
        printf("Create thread OK\r\n");
    } else {
        printf("Create thread err\r\n");
    }

TAR_OUT:

    if (sql_str) {
        free(sql_str);
    }
    return ret;
}

