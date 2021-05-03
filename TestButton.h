#ifndef TESTBUTTON_H
#define TESTBUTTON_H

#include <QPushButton>

class TestButton : public QPushButton
{
    Q_OBJECT
public:
    explicit TestButton(QWidget *parent = 0, int tid = 0);
    ~TestButton();

    int getTestID();


public slots:

private:
    int test_id;
};

#endif
