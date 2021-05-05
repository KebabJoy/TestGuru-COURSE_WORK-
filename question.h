#ifndef QUESTION_H
#define QUESTION_H


#include <QPushButton>

class Question : public QPushButton
{
    Q_OBJECT
public:
    explicit Question(QWidget *parent = 0, int qid = 0);
    ~Question();

    int getQuestionID();


public slots:

private:
    int question_id;
};
#endif // QUESTION_H
