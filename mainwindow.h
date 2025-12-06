#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "professordatabase.h"


#include <QMainWindow>
#include <QString>

class MainWindowView;
class CourseDatabase;
class ProfessorDatabase;
class UserAuth;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onMajorChanged(const QString &major);
    void onYearChanged(const QString &year);
    void onCourseChanged(const QString &courseCode);
    void onIsbnInputReturnPressed();

    void onLoginClicked();
    void onLogoutClicked();
    void onLoginSuccess(const QString &username);
private:
    void loadData();
    void setLoggedIn(bool loggedIn);
    void updateCourseDisplay(const QString &courseCode);
    void showProfessorBook(int index);


    int m_currentProfIndex = 0;
    QVector<ProfessorInfo> m_currentProfList;


    MainWindowView    *ui;
    CourseDatabase    *m_courseDb;
    ProfessorDatabase *m_profDb;
    UserAuth          *m_auth;

    bool               m_loggedIn;
    QString            m_currentUser;

};

#endif
