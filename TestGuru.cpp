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
        current_user = new User(checkUser.value(0).toInt(), emaill, checkUser.value(3).toInt());

        ui->Layout->setCurrentIndex(2);
        ui->welcome->setText(emaill);
    }else{
        ui->signinStatus->setText("User doen't exist");
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

void TestGuru::on_showTests_clicked()
{
    clearLayout(tests_lay);
    QSqlQuery tests(mydb);
    tests.exec("SELECT * FROM TESTS");

    while(tests.next()){
        QString str = tests.value(1).toString();


        TestButton *test_button = new TestButton(this, tests.value(0).toInt());
        test_button->setText(str);
        connect(test_button, SIGNAL(clicked()), this, SLOT(jump_to_test()));
        tests_lay->addWidget(test_button);

    }
    ui->scrollArea->setLayout(tests_lay);
}

void TestGuru::jump_to_test(){
    TestButton *button = (TestButton*) sender();

    current_test = button;
    start_test_passage();
    get_questions(button);
}

void TestGuru::start_test_passage(){
    QSqlQuery q_count;
    q_count.prepare("SELECT COUNT(*) FROM QUESTIONS WHERE test_id = (:tid)");
    q_count.bindValue(":tid", current_test->getTestID());
    q_count.exec();
    q_count.next();

    current_passage = new TestPassage(q_count.value(0).toInt());
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
        Answer *ans = new Answer(this, answers.value(0).toInt(), ans_body, answers.value(2).toBool());
        connect(ans, SIGNAL(clicked()), this, SLOT(assert_correctness()));
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

void TestGuru::assert_correctness(){
    Answer *current_ans = (Answer*) sender();
    if(current_ans->isCorrect()){
        current_passage->addCorrect();
    }

    next_question();
}

void TestGuru::next_question()
{
    if(current_question_query.next()){
        render_answers();
    }else{
        if(current_passage->isSuccess()){
            QSqlQuery test_success(mydb);
            test_success.prepare("INSERT INTO TEST_PASSAGES(test_id, user_id, passed) values(:tid, :uid, :ps)");
            test_success.bindValue(":tid", current_test->getTestID());
            test_success.bindValue(":uid", current_user->getUserID());
            test_success.bindValue(":ps", true);
            test_success.exec();

            ui->test_finish_header->setText("Поздравляем!");
            int sp = current_passage->successPercentage();
            ui->test_finish_result->setWordWrap(true);
            ui->test_finish_result->setText("Вы прошли тест с результатом: " + QString::number(sp) + "% \nВы всегда можете вернуться и пройти его снова.");
        }else{
            ui->test_finish_header->setText("Тест провален");
            int sp = current_passage->successPercentage();
            ui->test_finish_result->setWordWrap(true);
            ui->test_finish_result->setText("Вы провалили тест с результатом: " + QString::number(sp) + "% \nВы можете попробовать снова.");
        }
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
    clearLayout(ui->delete_test_layout);
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

        Question *question_button = new Question(this, questions.value(0).toInt(), str, questions.value(3).toString());
        question_button->setText(str);
        connect(question_button, SIGNAL(clicked()), this, SLOT(jump_to_edit_question_path()));

        questions_lay->addWidget(question_button);
    }

    ui->admin_questions->setLayout(questions_lay);
}

void TestGuru::jump_to_edit_question_path(){
    Question *button = (Question*) sender();

    current_question = button;

    ui->question_name->setText(current_question->getQuestionTitle());
    ui->edit_question_body->setWordWrap(true);
    ui->edit_question_body->setText(current_question->getQuestionBody());
    render_question_answers();
    ui->Layout->setCurrentIndex(8);
}

void TestGuru::on_admin_panel_clicked()
{
    ui->Layout->setCurrentIndex(5);
}

void TestGuru::delete_test(){

    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                           "Test Guru", "Вы уверены, что хотите удалить этот тест?"\
                                           "\nВсе связанные с ним вопросы и ответы будут также удалены.",
                                           QMessageBox::Yes|QMessageBox::No).exec()){
        TestButton *button = (TestButton*) sender();

        QSqlQuery del_test(mydb);
        del_test.prepare("DELETE FROM TESTS WHERE ID = (:tid)");
        del_test.bindValue(":tid", button->getTestID());
        del_test.exec();

        clearLayout(tests_lay);
        clearLayout(questions_lay);

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
    ui->new_test_status->setWordWrap(true);
    if(ui->new_test_title->text() == ""){
        ui->new_test_status->setText("Название теста не может быть пустым!");
    } else{
        QSqlQuery new_test(mydb);
        new_test.prepare("INSERT INTO TESTS(TITLE) VALUES(:tl)");
        new_test.bindValue(":tl", ui->new_test_title->text());
        new_test.exec();

        on_show_tests_clicked();
        ui->new_test_status->setText("Тест успешно создан");
    }
}

void TestGuru::render_question_answers(){
    clearLayout(answer_lay);
    clearLayout(ui->delete_question_layout);
    QSqlQuery answers(mydb);
    answers.prepare("SELECT * FROM ANSWERS where question_id = (:qid)");
    answers.bindValue(":qid", current_question->getQuestionID());
    answers.exec();
    ui->question_title->setText(current_question->getQuestionTitle());

    while(answers.next()){
        QString ans_body = answers.value(1).toString();

        Answer *ans = new Answer(this, answers.value(0).toInt(), ans_body);
        connect(ans, SIGNAL(clicked()), this, SLOT(jump_to_edit_answer_path()));

        ans->setText(ans_body);
        answer_lay->addWidget(ans);
    }
    Question *del_question_button = new Question(this, current_question->getQuestionID());
    connect(del_question_button, SIGNAL(clicked()), this, SLOT(delete_question()));
    del_question_button->setText("Delete " + del_question_button->getQuestionTitle());
    del_question_button->setStyleSheet("Padding: 1px;Border-radius: 5px;Background: #cc0000;Color: #fefefe;");
    ui->delete_question_layout->addWidget(del_question_button);

    ui->admin_answers->setLayout(answer_lay);
}

void TestGuru::delete_question(){
    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                           "Test Guru", "Вы уверены, что хотите удалить этот вопрос? Все связанные с ним ответы будут удалены",
                                           QMessageBox::Yes|QMessageBox::No).exec()){
        QSqlQuery del_q(mydb);
        del_q.prepare("DELETE FROM QUESTIONS WHERE ID = (:id)");
        del_q.bindValue(":id", current_question->getQuestionID());
        del_q.exec();
        render_questions();
        ui->Layout->setCurrentIndex(6);
    }
}

void TestGuru::on_edit_question_back_clicked()
{
    edit_test_path();
    ui->Layout->setCurrentIndex(6);
}

void TestGuru::on_update_question_clicked()
{
    QString tmp_title = ui->question_title_box->text();
    QString tmp_body = ui->question_body_box->text();
    QSqlQuery upd_q(mydb);
    if(tmp_body != "" && tmp_title != ""){
        upd_q.prepare("UPDATE QUESTIONS SET title = (:tl), body = (:bd) where id = (:qid)");
        upd_q.bindValue(":tl", tmp_title);
        upd_q.bindValue(":bd", tmp_body);
        upd_q.bindValue(":qid", current_question->getQuestionID());
        upd_q.exec();
        ui->question_name->setText(tmp_title);
        ui->edit_question_body->setText(tmp_body);
    }else if(tmp_body != ""){
        upd_q.prepare("UPDATE QUESTIONS SET body = (:bd) where id = (:qid)");
        upd_q.bindValue(":bd", tmp_body);
        upd_q.bindValue(":qid", current_question->getQuestionID());
        ui->edit_question_body->setText(tmp_body);
        upd_q.exec();
    } else if(tmp_title != ""){
        upd_q.prepare("UPDATE QUESTIONS SET title = (:tl) where id = (:qid)");
        upd_q.bindValue(":tl", tmp_title);
        upd_q.bindValue(":qid", current_question->getQuestionID());
        upd_q.exec();
        ui->question_name->setText(tmp_title);
    }
}

void TestGuru::on_new_answer_back_clicked()
{
    ui->Layout->setCurrentIndex(8);
}

void TestGuru::on_new_answer_button_clicked()
{
    QString tmp_b = ui->new_answer_body->text();
    if(tmp_b == ""){
        ui->new_answer_status->setText("Поле должно быть заполнено");
        ui->new_answer_status->setWordWrap(true);
    }else{
        QSqlQuery new_ans(mydb);
        if(ui->correct_check->isChecked()){
             new_ans.prepare("INSERT INTO ANSWERS(body, question_id, correct) values(:bd, :qid, :cr)");
             new_ans.bindValue(":cr", true);
        }else{
            new_ans.prepare("INSERT INTO ANSWERS(body, question_id) values(:bd, :qid)");
        }

        new_ans.bindValue(":qid", current_question->getQuestionID());
        new_ans.bindValue(":bd", tmp_b);
        new_ans.exec();

        QSqlQuery get_ans(mydb);
        get_ans.exec("SELECT ID FROM ANSWERS ORDER BY ID DESC LIMIT 1");
        get_ans.next();
        Answer *ans = new Answer(this, get_ans.value(0).toInt());
        connect(ans, SIGNAL(clicked()), this, SLOT(jump_to_edit_answer_path()));

        ans->setText(tmp_b);
        answer_lay->addWidget(ans);
        ui->Layout->setCurrentIndex(8);
    }
}

void TestGuru::on_add_answer_clicked()
{
    ui->Layout->setCurrentIndex(9);
}

void TestGuru::jump_to_edit_answer_path(){
    Answer *button = (Answer*) sender();
    current_answer = button;
    ui->edit_answer_name->setText(current_answer->getAnswerBody());

    ui->Layout->setCurrentIndex(10);
}

void TestGuru::on_edit_answer_back_clicked()
{
    ui->Layout->setCurrentIndex(8);
}

void TestGuru::on_edit_answer_submit_clicked()
{
    QString tmp_b = ui->edit_answer_body->text();
    if(tmp_b == ""){
        ui->edit_answer_status->setWordWrap(true);
        ui->edit_answer_status->setText("Поле должно быть заполнено");
    }else{
        QSqlQuery update_ans(mydb);
        update_ans.prepare("UPDATE ANSWERS SET body = (:bd) where id = (:id)");
        update_ans.bindValue(":bd", tmp_b);
        update_ans.bindValue(":id", current_answer->getAnswerId());
        update_ans.exec();

        render_question_answers();
        ui->Layout->setCurrentIndex(8);
    }
}

void TestGuru::on_edit_answer_delete_clicked()
{
    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                           "Test Guru", "Вы уверены, что хотите удалить этот ответ?",
                                           QMessageBox::Yes|QMessageBox::No).exec()){
        QSqlQuery del_ans(mydb);
        del_ans.prepare("DELETE FROM ANSWERS WHERE id = (:id)");
        del_ans.bindValue(":id", current_answer->getAnswerId());
        del_ans.exec();

        render_question_answers();
        ui->Layout->setCurrentIndex(8);
    }
}

void TestGuru::on_show_passed_tests_clicked()
{
    clearLayout(tests_lay);
    QSqlQuery passed_tests;
    passed_tests.prepare("select tests.title from users inner join test_passages on (users.id = user_id) inner join tests on (tests.id = test_id) where passed = 1 and users.id = :uid");
    passed_tests.bindValue(":uid", current_user->getUserID());

    passed_tests.exec();


    while(passed_tests.next()){
        QLabel *test  = new QLabel;
        test->setText(passed_tests.value(0).toString());
        tests_lay->addWidget(test);
        qDebug() << current_user->getUserID();
    }
    ui->scrollArea->setLayout(tests_lay);
}
