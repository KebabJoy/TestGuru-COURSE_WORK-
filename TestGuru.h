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
#include "testpassage.h"
#include "user.h"

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

    void on_back_2_clicked();

    void on_show_tests_clicked();

    void jump_to_edit_test_path();

    void edit_test_path();

    void on_admin_panel_clicked();

    void jump_to_edit_question_path();

    void delete_test();

    void on_add_question_clicked();

    void on_update_test_clicked();

    void on_edit_test_back_clicked();

    void on_back_to_user_panel_clicked();

    void on_new_question_button_clicked();

    void on_new_question_back_clicked();

    void render_questions();

    void on_new_test_clicked();

    void render_question_answers();

    void on_edit_question_back_clicked();

    void on_update_question_clicked();

    void on_new_answer_back_clicked();

    void on_new_answer_button_clicked();

    void on_add_answer_clicked();

    void jump_to_edit_answer_path();

    void on_edit_answer_back_clicked();

    void on_edit_answer_submit_clicked();

    void on_edit_answer_delete_clicked();

    void delete_question();

    void next_question();

    void start_test_passage();

    void assert_correctness();

    void on_show_passed_tests_clicked();

private:
    QGridLayout *answer_lay;
    QGridLayout *tests_lay;
    QGridLayout *questions_lay;
    QSqlQuery current_question_query;
    TestButton *current_test;
    Question *current_question;
    Answer *current_answer;
    TestPassage *current_passage;
    User *current_user;
    Ui::TestGuru *ui;
};
#endif // TESTGURU_H
