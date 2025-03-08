#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QCryptographicHash>


class utils
{
public:
    utils();
};


// Function Declaration
QString hashPassword(const QString &password);

#endif // UTILS_H
