#include "answer.h"

Answer::Answer(QWidget *parent, int aid, QString ans_b, bool crct)
   : QPushButton(parent)
{
    answer_id = aid;
    body = ans_b;
    correct = crct;
}

Answer::~Answer(){

}

int Answer::getAnswerId() const {
    return answer_id;
}

QString Answer::getAnswerBody() const{
    return body;
}

bool Answer::isCorrect() const{
    return correct;
}
