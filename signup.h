#ifndef SIGNUP_H
#define SIGNUP_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class SignUp; }
QT_END_NAMESPACE

class SignUp : public QMainWindow
{
    Q_OBJECT

public:
    SignUp(QWidget *parent = nullptr);
    ~SignUp();

private:
    Ui::SignUp *ui;
};
#endif // SIGNUP_H
