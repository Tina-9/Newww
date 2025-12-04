#include "userauth.h"
#include <QFile>
#include <QTextStream>

UserAuth::UserAuth(QObject *parent)
    : QObject(parent)
{
    // csv:username,password
    m_filePath = "users.txt";
}

bool UserAuth::login(const QString &username, const QString &password)
{
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        const QStringList parts = line.split(',');
        if (parts.size() != 2) continue;

        if (parts[0] == username && parts[1] == password)
            return true;
    }
    return false;
}

bool UserAuth::signUp(const QString &username, const QString &password)
{
    if (username.trimmed().isEmpty() || password.isEmpty())
        return false;


    if (login(username, password)) {
        return false;
    }

    QFile file(m_filePath);
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out << username << "," << password << "\n";
    return true;
}
