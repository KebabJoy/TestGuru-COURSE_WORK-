#include "question.h"

Question::Question(QWidget *parent, int qid) :
    QPushButton(parent)
{
    question_id = qid;
}

Question::~Question()
{

}


int Question::getQuestionID()
{
    return question_id;
}
