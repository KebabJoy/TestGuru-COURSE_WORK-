#ifndef USER_H
#define USER_H

#include<QString>
class User
{
public:
    explicit User(int id = 0, QString email = 0, int rank = 0);

    int getUserID() const;

    QString getUserEmail() const;

    int getUserRank() const;

private:
    int user_id;
    QString email;
    int rank;
};

#endif // USER_H
