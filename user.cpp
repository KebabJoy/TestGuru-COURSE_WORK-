#include "user.h"

User::User(int id, QString email, int rank) : email(email), rank(rank)
{
    user_id = id;
}

QString User::getUserEmail() const{
    return email;
}

int User::getUserID() const{
    return user_id;
}

int User::getUserRank() const{
    return rank;
}
