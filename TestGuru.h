#ifndef TESTGURU_H
#define TESTGURU_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QGridLayout>
#include <QMessageBox>


#include "TestButton.h"
#include "answer.h"
#include "question.h"

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

    void get_questions(TestButton *button);

    void render_answers();

    void on_back_clicked();

    void on_next_ans_clicked();

    void on_back_2_clicked();

    void on_show_tests_clicked();

    void jump_to_edit_test_path();

    void edit_test_path(TestButton *test);

    void on_admin_panel_clicked();

    void jump_to_edit_question_path();

    void delete_test();

private:
    QGridLayout *answer_lay;
    QSqlQuery current_question;
    int rank;
    QString email;
    QString passwd;
    Ui::TestGuru *ui;
};
#endif // TESTGURU_H
