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
}

TestGuru::~TestGuru()
{
    delete ui;
}


void TestGuru::on_pushButton_clicked()
{
    QSqlQuery newUser(mydb);
    newUser.prepare("INSERT INTO USERS(email, passwd)VALUES(:em, :pw)");
    newUser.bindValue(":em", ui->emailBox->text());
    newUser.bindValue(":pw", ui->passBox->text());
    newUser.exec();
}
