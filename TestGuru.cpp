#include "TestGuru.h"
#include "ui_TestGuru.h"

TestGuru::TestGuru(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestGuru)
{
    ui->setupUi(this);

    mydb=QSqlDatabase::addDatabase("QSQLITE");

    mydb.setDatabaseName("/home/kebabjoy/Документы/КУРСАЧ/TestGuru/development.sqlite3");
    if(!mydb.open())
        qDebug() << "DB is not connected";
    mydb.exec("PRAGMA foreign_keys = ON");

    ui->passBox->setEchoMode(QLineEdit::Password);
    ui->passConfBox->setEchoMode(QLineEdit::Password);
    ui->signinPass->setEchoMode(QLineEdit::Password);

    answer_lay = new QGridLayout;
    tests_lay = new QGridLayout;
    questions_lay = new QGridLayout;
}

TestGuru::~TestGuru()
{
    delete ui;
}


void TestGuru::on_pushButton_clicked()
{
    QSqlQuery checkUser(mydb);
    checkUser.prepare("SELECT * FROM USERS WHERE email = :em");
    QString pass = ui->passBox->text();
    checkUser.bindValue(":em", ui->emailBox->text());
    bool exists = false;

    if(checkUser.exec() ){
        if(checkUser.next()){
            ui->regStatus->setText("User with this email exists");
            exists = true;
        }
    }
    if(!exists){
        if(pass == ui->passConfBox->text()){
            QSqlQuery newUser(mydb);
            newUser.prepare("INSERT INTO USERS(email, passwd)VALUES(:em, :pw)");
            newUser.bindValue(":em", ui->emailBox->text());
            newUser.bindValue(":pw", pass);
            newUser.exec();

            ui->regStatus->setText("Account successfully created");

            this->passwd = pass;
            this->email = ui->emailBox->text();
            this->rank = 0;
        }
    }
}


void TestGuru::on_pushButton_4_clicked()
{
    ui->Layout->setCurrentIndex(1);
}

void TestGuru::on_pushButton_5_clicked()
{
    ui->Layout->setCurrentIndex(0);
}



void TestGuru::on_pushButton_6_clicked()
{
    QSqlQuery checkUser(mydb);
    checkUser.prepare("SELECT * FROM USERS WHERE email = (:em) AND passwd = (:pw)");

    QString pass = ui->signinPass->text();
    QString emaill = ui->signinEmail->text();

    checkUser.bindValue(":em", emaill);
    checkUser.bindValue(":pw", pass);
    bool exists = false;

    if(checkUser.exec()){
        if(checkUser.next())
        {
            exists = true;
        }
    }

    if(exists){
        this->passwd = pass;
        this->email = emaill;

        ui->Layout->setCurrentIndex(2);
        ui->welcome->setText(emaill);
    }else{
        ui->signinStatus->setText("User doen't exist");
    }

}


void TestGuru::on_showTests_clicked()
{

    QSqlQuery tests(mydb);
    tests.exec("SELECT * FROM TESTS");

    QGridLayout *lay = new QGridLayout;
    while(tests.next()){
        QString str = tests.value(1).toString();


        TestButton *test_button = new TestButton(this, tests.value(0).toInt());
        test_button->setText(str);
        connect(test_button, SIGNAL(clicked()), this, SLOT(jump_to_test()));
        lay->addWidget(test_button);

    }
    ui->scrollArea->setLayout(lay);
}

void TestGuru::jump_to_test(){
    TestButton *button = (TestButton*) sender();

    get_questions(button);
}


void TestGuru::get_questions(TestButton *button){
    QSqlQuery questions(mydb);
    questions.prepare("SELECT * FROM QUESTIONS where test_id = (:ti)");
    questions.bindValue(":ti", button->getTestID());
    questions.exec();
    current_question_query = questions;

    if(current_question_query.next()){
        render_answers();
        ui->Layout->setCurrentIndex(3);
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("The test is not finished. Please try to pass other.");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
}

void clearLayout(QLayout *layout) {
    if (layout) {
           while(layout->count() > 0){
               QLayoutItem *item = layout->takeAt(0);
               QWidget* widget = item->widget();
               if(widget)
                   delete widget;
               delete item;
           }
    }
}

void TestGuru::render_answers(){

    clearLayout(answer_lay);
    QSqlQuery answers(mydb);
    answers.prepare("SELECT * FROM ANSWERS where question_id = (:qid)");
    answers.bindValue(":qid", current_question_query.value(0).toInt());
    answers.exec();
    ui->question_title->setText(current_question_query.value(1).toString());

    while(answers.next()){
        QString ans_body = answers.value(1).toString();
        qDebug() << "lol";
        Answer *ans = new Answer(this, answers.value(0).toInt());
        ans->setText(ans_body);
        answer_lay->addWidget(ans);
    }
    ui->answers->setLayout(answer_lay);

}



void TestGuru::on_back_clicked()
{
    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                           "Test Guru", "Are you sure you want move back to all questions?"\
                                           "\nThis action will erase your progress",
                                           QMessageBox::Yes|QMessageBox::No).exec()){
        clearLayout(answer_lay);
        ui->Layout->setCurrentIndex(2);
    }
}

void TestGuru::on_next_ans_clicked()
{
    if(current_question_query.next()){
        render_answers();
    }else{
        clearLayout(answer_lay);
        ui->Layout->setCurrentIndex(4);
    }
}

void TestGuru::on_back_2_clicked()
{
    clearLayout(answer_lay);
    ui->Layout->setCurrentIndex(2);
}

void TestGuru::on_show_tests_clicked()
{
    clearLayout(tests_lay);
    QSqlQuery tests(mydb);
    tests.exec("SELECT * FROM TESTS");

    while(tests.next()){
        QString str = tests.value(1).toString();


        TestButton *test_button = new TestButton(this, tests.value(0).toInt(), tests.value(1).toString());
        test_button->setText(str);
        connect(test_button, SIGNAL(clicked()), this, SLOT(jump_to_edit_test_path()));
        tests_lay->addWidget(test_button);

    }
    ui->admin_tests_list->setLayout(tests_lay);
}

void TestGuru::jump_to_edit_test_path(){
    TestButton *button = (TestButton*) sender();

    current_test = button;
    edit_test_path();
    ui->Layout->setCurrentIndex(6);
}

void TestGuru::edit_test_path(){
    ui->test_name->setText(current_test->getTestTitle());

    render_questions();

    TestButton *del_test_button = new TestButton(this, current_test->getTestID(), current_test->getTestTitle());
    connect(del_test_button, SIGNAL(clicked()), this, SLOT(delete_test()));
    del_test_button->setText("Delete " + del_test_button->getTestTitle());
    del_test_button->setStyleSheet("Padding: 1px;Border-radius: 5px;Background: #cc0000;Color: #fefefe;");

    ui->delete_test_layout->addWidget(del_test_button);

    ui->edit_test_status->setText("");


}

void TestGuru::render_questions(){
    clearLayout(questions_lay);
    QSqlQuery questions(mydb);
    questions.prepare("SELECT * FROM QUESTIONS where test_id = (:tid)");
    questions.bindValue(":tid", current_test->getTestID());
    questions.exec();

    while(questions.next()){
        QString str = questions.value(1).toString();

        Question *question_button = new Question(this, questions.value(0).toInt());
        question_button->setText(str);
        connect(question_button, SIGNAL(clicked()), this, SLOT(jump_to_edit_question_path()));

        questions_lay->addWidget(question_button);
    }

    ui->admin_questions->setLayout(questions_lay);
}

void TestGuru::jump_to_edit_question_path(){
    Question *button = (Question*) sender();

    current_question = button;

}

void TestGuru::on_admin_panel_clicked()
{
    ui->Layout->setCurrentIndex(5);
}

void TestGuru::delete_test(){

    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                           "Test Guru", "Do you want to delete this test?"\
                                           "\nThis action will delete all questions and answers connected to it.",
                                           QMessageBox::Yes|QMessageBox::No).exec()){
        TestButton *button = (TestButton*) sender();

        QSqlQuery del_test(mydb);
        del_test.prepare("DELETE FROM TESTS WHERE ID = (:tid)");
        del_test.bindValue(":tid", button->getTestID());
        del_test.exec();

        clearLayout(tests_lay);
        clearLayout(questions_lay);
        clearLayout(ui->delete_test_layout);
        ui->Layout->setCurrentIndex(5);
    }
}



void TestGuru::on_add_question_clicked()
{
    ui->Layout->setCurrentIndex(7);
}


void TestGuru::on_update_test_clicked()
{
    QString btitle = ui->test_title_box->text();
    if(btitle != ""){
        QSqlQuery new_title(mydb);
        new_title.prepare("UPDATE TESTS SET TITLE = (:ti) WHERE id = (:tid)");
        new_title.bindValue(":ti", btitle);
        new_title.bindValue(":tid", current_test->getTestID());
        new_title.exec();
        ui->test_name->setText(btitle);
    }else{
        ui->edit_test_status->setText("Название не может быть пустым");
        ui->edit_test_status->setWordWrap(true);
    }
}

void TestGuru::on_edit_test_back_clicked()
{
    clearLayout(questions_lay);
    clearLayout(tests_lay);
    clearLayout(ui->delete_test_layout);
    ui->Layout->setCurrentIndex(5);
}

void TestGuru::on_back_to_user_panel_clicked()
{
        ui->Layout->setCurrentIndex(2);
}

void TestGuru::on_new_question_button_clicked()
{
    if(ui->new_question_title->text() == "" || ui->new_question_body->text() == ""){
        ui->new_question_status->setWordWrap(true);
        ui->new_question_status->setText("Все поля должны быть заполнены");
    }else{
        QSqlQuery new_question;
        new_question.prepare("INSERT INTO QUESTIONS(title, body, test_id) VALUES(:tl, :bd, :ti)");
        new_question.bindValue(":ti", current_test->getTestID());
        new_question.bindValue(":tl", ui->new_question_title->text());
        new_question.bindValue(":bd", ui->new_question_body->text());
        new_question.exec();


        render_questions();

        ui->Layout->setCurrentIndex(6);
    }
}

void TestGuru::on_new_question_back_clicked()
{
    ui->Layout->setCurrentIndex(6);
}

void TestGuru::on_new_test_clicked()
{
    if(ui->new_test_title->text() == ""){
        ui->new_test_status->setText("Название теста не может быть пустым!");
    } else{
        QSqlQuery new_test(mydb);
        new_test.prepare("INSERT INTO TESTS(TITLE) VALUES(:tl)");
        new_test.bindValue(":tl", ui->new_test_title->text());
        new_test.exec();

        on_show_tests_clicked();
        ui->new_test_status->setWordWrap(true);
        ui->new_test_status->setText("Тест успешно создан");
    }
}
