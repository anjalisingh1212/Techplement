#include "utils.h"

QString hashPassword(const QString &password) {
    QByteArray hashedData = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return hashedData.toHex();
}

