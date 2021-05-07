#ifndef TESTPASSAGE_H
#define TESTPASSAGE_H

#include <QString>

class TestPassage
{
public:
    explicit TestPassage(int qc = 0);

    void addCorrect();

    int successPercentage() const;

    bool isSuccess() const;

private:
    int questions_count;
    int correct_answers;
};

#endif // TESTPASSAGE_H
