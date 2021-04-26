#include "TestGuru.h"
#include "ui_TestGuru.h"

TestGuru::TestGuru(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestGuru)
{
    ui->setupUi(this);

    mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("/home/kebabjoy/TestGuru/development.sqlite3");
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

void TestGuru::on_stackedWidget_currentChanged(int arg1)
{

}
