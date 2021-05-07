#ifndef QUESTION_H
#define QUESTION_H


#include <QPushButton>

class Question : public QPushButton
{
    Q_OBJECT
public:
    explicit Question(QWidget *parent = 0, int qid = 0, QString ttl = 0, QString bd = 0);
    ~Question();

    int getQuestionID() const;

    QString getQuestionTitle() const;

    QString getQuestionBody() const;


public slots:

private:
    int question_id;
    QString title;
    QString body;
};
#endif // QUESTION_H
