#include "TestButton.h"

TestButton::TestButton(QWidget *parent, int tid, QString ttl) :
    QPushButton(parent)
{
    test_id = tid;
    title = ttl;
}

TestButton::~TestButton()
{

}


int TestButton::getTestID() const
{
    return test_id;
}

QString TestButton::getTestTitle() const{
    return title;
}



