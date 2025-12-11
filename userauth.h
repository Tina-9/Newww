#ifndef USERAUTH_H
#define USERAUTH_H

#include <QObject>
#include <QString>
#include "authbase.h"

class UserAuth :  public QObject, public AuthBase
{
    Q_OBJECT
public:
    explicit UserAuth(QObject *parent = nullptr);

    bool login(const QString &username, const QString &password);
    bool signUp(const QString &username, const QString &password);

    bool validate(const QString &username, const QString &password) override
    {
        return login(username, password);
    }
private:
    QString m_filePath;
};

#endif
