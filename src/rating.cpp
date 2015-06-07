#include "rating.h"


unsigned int RatingScale::maxRating() const
{
    return _maxRating;
}

void RatingScale::setMaxRating(unsigned int maxRating)
{
    _maxRating = maxRating;
}

int RatingScale::minRating() const
{
    return -(int)maxRating() + 1;
}

QString RatingScale::ratingToString(int rating) const
{
    if(!ratingIsCorrect(rating))
        return QString();
    if(rating >= 0)
        return QString::number(rating + 1);
    else
        return QString("1/%1").arg(qAbs(rating) + 1);
}

double RatingScale::ratingToDouble(int rating) const
{
    if(rating >= 0)
        return rating + 1;
    else
        return (double)1 / (qAbs(rating) + 1);
}

QStringList RatingScale::allRating() const
{
    QStringList result;
    for(int i = minRating(); i < (int)maxRating(); i++)
        result << ratingToString(i);
    return result;
}

int RatingScale::numberToRating(int number) const
{
    int result = number + minRating();
    if(ratingIsCorrect(result))
        return result;
    return 0;
}

int RatingScale::ratingToNumber(int rating) const
{
    if(!ratingIsCorrect(rating))
        return ratingToNumber(0);
    return rating + maxRating() - 1;
}

bool RatingScale::ratingIsCorrect(int rating) const
{
    if(qAbs(rating) < (int)_maxRating)
        return true;
    return false;
}

RatingScale::RatingScale():
    _maxRating(9)
{    
}

RatingScale::RatingScale(unsigned int maxRating):
    _maxRating(maxRating)
{
}

