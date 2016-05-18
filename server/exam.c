#include "exam.h"
#include "ex_db.h"
typedef struct _question {
    int question_type;
    char major[32];
    char question[2048];
    char answer[64];
    struct list_head link_list;
} question_t;


static int add_paper_to_db(struct list_head *question_list_head)
{
    int ret;
    struct list_head *p_pos, *p_list;
    question_t *q;
    int question_id;

    if (!question_list_head) {
        ret = PARM_NULL;
    }
    if (list_empty_careful(question_list_head)) {
        ret = PARM_EMPTY;
        printf("Question list is empty");
        goto TAR_OUT;
    }
    //open_paper_db();
    list_for_each_safe(p_pos, p_list, question_list_head) {
        q = list_entry(p_pos， question_t， &link_list);
        insert_to_paperdb(question_id, q->question_type, q->major, q->question, q->answer);
        question_id++;
    }


TAR_OUT:
    return ret;
}

int genrate_paper(char *major)
{
    int ret;

    loading_exam();
    return ret;
}


