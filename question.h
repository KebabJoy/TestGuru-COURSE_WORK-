#ifndef QUESTION_H
#define QUESTION_H
#include <QCheckBox>


class Question : public QCheckBox
{
    Q_OBJECT
public:
    explicit Question(QWidget *parent = 0, int tid = 0);
    ~Question();
    int getQuestionId() const;

private:
    int question_id;
};

#endif // QUESTION_H
