#ifndef TESTGURU_H
#define TESTGURU_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class TestGuru; }
QT_END_NAMESPACE

class TestGuru : public QMainWindow
{
    Q_OBJECT

public:
    TestGuru(QWidget *parent = nullptr);
    ~TestGuru();

private:
    Ui::TestGuru *ui;
};
#endif // TESTGURU_H
