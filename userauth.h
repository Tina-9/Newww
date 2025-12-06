#ifndef USERAUTH_H
#define USERAUTH_H

#include <QObject>
#include <QString>

class UserAuth : public QObject
{
    Q_OBJECT
public:
    explicit UserAuth(QObject *parent = nullptr);

    bool login(const QString &username, const QString &password);
    bool signUp(const QString &username, const QString &password);

private:
    QString m_filePath;   // users.txt
};

#endif
