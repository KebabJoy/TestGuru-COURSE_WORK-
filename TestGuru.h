#ifndef TESTGURU_H
#define TESTGURU_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QGridLayout>

#include "qdynamicbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TestGuru; }
QT_END_NAMESPACE

class TestGuru : public QMainWindow
{
    Q_OBJECT

public:
    TestGuru(QWidget *parent = nullptr);
    ~TestGuru();
    QSqlDatabase mydb;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void jump_to_test();

    void on_showTests_clicked();

private:
    int rank;
    QString email;
    QString passwd;
    Ui::TestGuru *ui;
};
#endif // TESTGURU_H
