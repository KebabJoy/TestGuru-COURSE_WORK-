#ifndef ANSWER_H
#define ANSWER_H
#include <QPushButton>


class Answer : public QPushButton
{
    Q_OBJECT
public:
    explicit Answer(QWidget *parent = 0, int aid = 0, QString ans_b = 0, bool crct = 0);
    ~Answer();
    int getAnswerId() const;

    QString getAnswerBody() const;

    bool isCorrect() const;


private:
    int answer_id;
    QString body;
    bool correct;
};

#endif // ANSWER_H
