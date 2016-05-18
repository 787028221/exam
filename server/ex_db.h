#ifndef DB_H
#define DB_H

enum {
    DB_CON_ERR = -100,
};

int init_db();
int close_db();
int reg_stu_to_db(char *name, char *stu_id, char *passwd);
int reg_admin(char *name, char *id, char *passwd);
int reg_teacher(char *name, char *id, char *passwd);
int serach_stu(char *stu_id, char *passwd);
int serach_admin(char *stu_id, char *passwd);
int serach_teacher(char *teacher_id, char *passwd);
int update_stu_passwd(char *stu_id, char *passwd, int len);
int update_stu_name(char *stu_id, char *name);
int update_teacher_passwd(char *teacher_id, char *passwd, int len);
int update_teacher_name(char *teacher_id, char *name, int len);
int update_admin_passwd(char *teacher_id, char *passwd, int len);
int update_admin_name(char *teacher_id, char *name,  int len);
int send_teacher_msg_to_client(int fd);
int send_student_msg_to_client(int fd);
#endif // DB_H
