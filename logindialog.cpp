#include "logindialog.h"
#include "userauth.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

LoginDialog::LoginDialog(UserAuth *auth, QWidget *parent)
    : QDialog(parent), m_auth(auth)
{
    setWindowTitle("Login / Sign Up");
    setModal(true);
    setMinimumWidth(320);

    auto *lblUser = new QLabel("Username:");
    auto *lblPass = new QLabel("Password:");

    editUser = new QLineEdit;
    editPass = new QLineEdit;
    editPass->setEchoMode(QLineEdit::Password);

    btnLogin  = new QPushButton("Login");
    btnSignUp = new QPushButton("Sign Up");

    auto *formLayout = new QGridLayout;
    formLayout->addWidget(lblUser, 0, 0);
    formLayout->addWidget(editUser, 0, 1);
    formLayout->addWidget(lblPass, 1, 0);
    formLayout->addWidget(editPass, 1, 1);

    auto *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnLogin);
    btnLayout->addWidget(btnSignUp);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);

    connect(btnLogin,  &QPushButton::clicked,this,&LoginDialog::onLoginClicked);
    connect(btnSignUp, &QPushButton::clicked,this,&LoginDialog::onSignUpClicked);
}

void LoginDialog::onLoginClicked()
{
    const QString user = editUser->text().trimmed();
    const QString pass = editPass->text();

    if (m_auth->login(user, pass))
    {
        QMessageBox::information(this, "Success", "Login successful.");
        emit loginSuccess(user);
        accept();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Wrong username or password.");
    }
}

void LoginDialog::onSignUpClicked()
{
    const QString user = editUser->text().trimmed();
    const QString pass = editPass->text();

    if (m_auth->signUp(user, pass))
    {
        QMessageBox::information(this, "Success", "Account created! You can login now.");
    } else {
        QMessageBox::warning(this, "Error", "Could not create account (maybe already exists?).");
    }
}
