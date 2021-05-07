#include "question.h"

Question::Question(QWidget *parent, int qid, QString ttl, QString bd) :
    QPushButton(parent)
{
    question_id = qid;
    title = ttl;
    body = bd;
}

Question::~Question()
{

}


int Question::getQuestionID() const
{
    return question_id;
}

QString Question::getQuestionTitle() const{
    return title;
}

QString Question::getQuestionBody() const{
    return body;
}
