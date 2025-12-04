#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class UserAuth;

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(UserAuth *auth, QWidget *parent = nullptr);

signals:
    void loginSuccess(const QString &username);

private slots:
    void onLoginClicked();
    void onSignUpClicked();

private:
    UserAuth *m_auth;
    QLineEdit *editUser;
    QLineEdit *editPass;
    QPushButton *btnLogin;
    QPushButton *btnSignUp;
};

#endif // LOGINDIALOG_H
