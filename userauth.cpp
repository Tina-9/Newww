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
    QFile file(m_filePath);                                             // file input
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);                                              // reading stream
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();                         // reading line
        if (line.isEmpty()) continue;

        const QStringList parts = line.split(',');                      // Split username,password， just same as the professordatabase.cpp
        if (parts.size() != 2) continue;

        if (parts[0] == username && parts[1] == password)               // Login success
            return true;
    }
    return false;                                                       // Not found
}

bool UserAuth::signUp(const QString &username, const QString &password)
{
    if (username.trimmed().isEmpty() || password.isEmpty())
        return false;


    if (login(username, password)) {                                    // User exists
        return false;
    }

    QFile file(m_filePath);                                             // File output
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream out(&file);                                             // Writing stream
    out << username << "," << password << "\n";                         // Append new user
    return true;
}
