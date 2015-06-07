#ifndef RATING_H
#define RATING_H

#include <QString>
#include <QStringList>

class RatingScale
{
    unsigned int _maxRating;
public:
    RatingScale();
    RatingScale(unsigned int maxRating);

    unsigned int maxRating() const;
    void setMaxRating(unsigned int maxRating);

    int minRating() const;

    bool ratingIsCorrect(int rating) const;
    QString ratingToString(int rating) const;
    double ratingToDouble(int rating) const;
    QStringList allRating() const;

    int numberToRating(int number) const;
    int ratingToNumber(int rating) const;
};

#endif // RATING_H
