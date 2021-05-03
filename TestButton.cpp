#include "TestButton.h"

TestButton::TestButton(QWidget *parent, int tid) :
    QPushButton(parent)
{
    test_id = tid;
}

TestButton::~TestButton()
{

}


int TestButton::getTestID()
{
    return test_id;
}



