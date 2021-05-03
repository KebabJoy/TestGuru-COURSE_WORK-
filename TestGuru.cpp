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


        TestButton *button = new TestButton(this, tests.value(0).toInt());
        button->setText(str);
        connect(button, SIGNAL(clicked()), this, SLOT(jump_to_test()));
        lay->addWidget(button);

    }
    ui->scrollArea->setLayout(lay);
}

void TestGuru::jump_to_test(){
    TestButton *button = (TestButton*) sender();


    ui->Layout->setCurrentIndex(3);
    TestGuru::render_questions(button);
}


void TestGuru::render_questions(TestButton *button){
    QSqlQuery questions(mydb);
    questions.prepare("SELECT * FROM QUESTIONS where test_id = (:ti)");
    questions.bindValue(":ti", button->getTestID());
    questions.exec();


    QGridLayout *lay = new QGridLayout;
    while(questions.next()){
        QString str = questions.value(1).toString();
        qDebug() << "lol";

        TestButton *button = new TestButton(this);
        button->setText(str);
        connect(button, SIGNAL(clicked()), this, SLOT(jump_to_test()));
        lay->addWidget(button);
    }
     ui->questions->setLayout(lay);
}

void TestGuru::on_back_clicked()
{
    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                           "Test Guru", "Are you sure you want move back to all questions?"\
                                           "\nThis action will erase your progress",
                                           QMessageBox::Yes|QMessageBox::No).exec()){
        ui->Layout->setCurrentIndex(2);
    }
}
