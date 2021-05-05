#ifndef ANSWER_H
#define ANSWER_H
#include <QCheckBox>


class Answer : public QCheckBox
{
    Q_OBJECT
public:
    explicit Answer(QWidget *parent = 0, int aid = 0);
    ~Answer();
    int getAnswerId() const;


private:
    int answer_id;
};

#endif // ANSWER_H
