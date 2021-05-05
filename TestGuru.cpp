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

    ui->passBox->setEchoMode(QLineEdit::Password);
    ui->passConfBox->setEchoMode(QLineEdit::Password);
    ui->signinPass->setEchoMode(QLineEdit::Password);

    answer_lay = new QGridLayout;
    QSqlQuery test(mydb);
    test.prepare("SELECT * FROM USERS WHERE email = :em");
    test.bindValue(":em","maximka@mail.ru");
    test.exec();
    test.next();
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
    current_question = questions;

    if(current_question.next()){
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
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
           delete item->widget();
        }
        delete item;
    }
}

void TestGuru::render_answers(){

    QSqlQuery answers(mydb);
    answers.prepare("SELECT * FROM ANSWERS where question_id = (:qid)");
    answers.bindValue(":qid", current_question.value(0).toInt());
    answers.exec();
    ui->question_title->setText(current_question.value(1).toString());

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
    if(current_question.next()){
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
    QSqlQuery tests(mydb);
    tests.exec("SELECT * FROM TESTS");

    QGridLayout *lay = new QGridLayout;
    while(tests.next()){
        QString str = tests.value(1).toString();


        TestButton *test_button = new TestButton(this, tests.value(0).toInt(), tests.value(1).toString());
        test_button->setText(str);
        connect(test_button, SIGNAL(clicked()), this, SLOT(jump_to_edit_test_path()));
        lay->addWidget(test_button);

    }
    ui->admin_tests_list->setLayout(lay);
}

void TestGuru::jump_to_edit_test_path(){
    TestButton *button = (TestButton*) sender();

    edit_test_path(button);
    ui->Layout->setCurrentIndex(6);
}

void TestGuru::edit_test_path(TestButton *test){
    ui->test_name->setText(test->getTestTitle());

    QSqlQuery questions(mydb);
    questions.prepare("SELECT * FROM QUESTIONS where test_id = (:tid)");
    questions.bindValue(":tid", test->getTestID());
    questions.exec();

    QGridLayout *lay = new QGridLayout;
    while(questions.next()){
        QString str = questions.value(1).toString();

        Question *question_button = new Question(this, questions.value(0).toInt());
        question_button->setText(str);

        connect(question_button, SIGNAL(clicked()), this, SLOT(jump_to_edit_question_path()));

        lay->addWidget(question_button);
    }
    connect(test, SIGNAL(clicked()), this, SLOT(delete_test()));
    test->setText("Delete " + test->getTestTitle());
    test->setStyleSheet("Padding: 1px;Border-radius: 5px;Background: #cc0000;Color: #fefefe;");
    ui->delete_test_layout->addWidget(test);

    ui->admin_answers->setLayout(lay);
}

void TestGuru::jump_to_edit_question_path(){
    Question *button = (Question*) sender();


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

    }
}


