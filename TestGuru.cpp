#include "TestGuru.h"
#include "ui_TestGuru.h"

TestGuru::TestGuru(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestGuru)
{
    ui->setupUi(this);

    QSqlDatabase mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("/home/kebabjoy/TestGuru/development.sqlite3");
    if(!mydb.open())
        qDebug() << "DB is not connected";
}

TestGuru::~TestGuru()
{
    delete ui;
}

