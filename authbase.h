#ifndef AUTHBASE_H
#define AUTHBASE_H

#include <QString>

class AuthBase
{
public:
    AuthBase() {}
    virtual ~AuthBase() {}


    virtual bool validate(const QString &username, const QString &password) = 0;
};

#endif
