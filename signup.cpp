#include "signup.h"
#include "ui_signup.h"

SignUp::SignUp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SignUp)
{
    ui->setupUi(this);

    QSqlDatabase mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("/home/kebabjoy/TestGuru/development.sqlite3");
    if(!mydb.open())
        ui->label->setText("Db is not connected");
    else
        ui->label->setText("Db is connected");
}

SignUp::~SignUp()
{
    delete ui;
}

