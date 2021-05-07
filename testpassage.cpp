#include "testpassage.h"

TestPassage::TestPassage(int qc)
{
    questions_count = qc;
    correct_answers = 0;
}

void TestPassage::addCorrect(){
    correct_answers++;
}

int TestPassage::successPercentage() const{
    return (int)(((double)correct_answers / (double)questions_count)*100);
}

bool TestPassage::isSuccess() const{
    return successPercentage()*100 > 80;
}
