#ifndef MAINWINDOWVIEW_H
#define MAINWINDOWVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QTableWidget>
#include <QTextEdit>

class MainWindowView : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindowView(QWidget *parent = nullptr);

    QPushButton *btnLogin;
    QPushButton *btnSignUp;
    QPushButton *btnLogout;

    QLineEdit *isbnInput;
    QComboBox *majorBox;
    QComboBox *yearBox;
    QComboBox *courseBox;

    QLabel *courseLabel;
    QLabel *isbnLabel;
    QLabel *coverLabel;

    QLabel *titleLabel;
    QLabel *authorLabel;


    QTableWidget *profTable;
    QTextEdit    *summaryEdit;
};

#endif
