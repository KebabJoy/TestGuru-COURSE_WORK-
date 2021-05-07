#include "answer.h"

Answer::Answer(QWidget *parent, int aid)
   : QPushButton(parent)
{
    answer_id = aid;
}

Answer::~Answer(){

}

int Answer::getAnswerId() const {
    return answer_id;
}
