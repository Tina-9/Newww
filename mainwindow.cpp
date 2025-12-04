#include "mainwindow.h"
#include "mainwindowview.h"
#include "coursedatabase.h"
#include "professordatabase.h"
#include "userauth.h"
#include "logindialog.h"

#include <QMessageBox>
#include <QDir>
#include <QPixmap>
#include <QDebug>
#include <QTableWidgetItem>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new MainWindowView(this))
    , m_courseDb(new CourseDatabase(this))
    , m_profDb(new ProfessorDatabase(this))
    , m_auth(new UserAuth(this))
    , m_loggedIn(false)
{
    setWindowTitle("StudyAhead - Course & Book Matcher");
    resize(1500, 900);
    setCentralWidget(ui);

    loadData();

    qDebug() << "Working Directory:" << QDir::currentPath();
    qDebug() << "courses.csv exists?" << QFile::exists("data/courses.csv");
    qDebug() << "professors.csv exists?" << QFile::exists("data/professors.csv");
    qDebug() << "users.txt exists?" << QFile::exists("data/users.txt");

    // Connect signals
    connect(ui->majorBox,  &QComboBox::currentTextChanged,this,&MainWindow::onMajorChanged);

    connect(ui->yearBox,   &QComboBox::currentTextChanged,this,&MainWindow::onYearChanged);

    connect(ui->courseBox, &QComboBox::currentTextChanged,this,&MainWindow::onCourseChanged);

    connect(ui->isbnInput, &QLineEdit::returnPressed,this,&MainWindow::onIsbnInputReturnPressed);

    connect(ui->btnLogin,  &QPushButton::clicked,this,&MainWindow::onLoginClicked);

    connect(ui->btnLogout, &QPushButton::clicked,this,&MainWindow::onLogoutClicked);

    setLoggedIn(false);
}

MainWindow::~MainWindow()
{
}

void MainWindow::loadData()
{
    if (!m_courseDb->loadFromCsv("data/courses.csv"))
    {
        QMessageBox::warning(this, "Error", "Could not load courses.csv");
    }
    if (!m_profDb->loadFromCsv("data/professors.csv"))
    {
        QMessageBox::warning(this, "Error", "Could not load professors.csv");
    }

    ui->majorBox->clear();
    ui->majorBox->addItem("Select Major");

    const QStringList majors = m_courseDb->majors();
    for (const QString &m : majors)
        ui->majorBox->addItem(m);
}

void MainWindow::setLoggedIn(bool loggedIn)
{
    m_loggedIn = loggedIn;

    ui->majorBox->setEnabled(loggedIn);
    ui->yearBox->setEnabled(loggedIn);
    ui->courseBox->setEnabled(loggedIn);
    ui->isbnInput->setEnabled(loggedIn);

    ui->btnLogin->setEnabled(!loggedIn);
    ui->btnLogout->setEnabled(loggedIn);

    if (!loggedIn)
    {
        ui->courseLabel->setText("Course: -");
        ui->isbnLabel->setText("ISBN: -");
        ui->coverLabel->setText("No Cover");
        ui->coverLabel->setPixmap(QPixmap());
        ui->summaryEdit->clear();
        ui->profTable->setRowCount(0);
    }
}

void MainWindow::onMajorChanged(const QString &major)
{
    if (major == "Select Major")
    {
        ui->yearBox->clear();
        ui->yearBox->addItem("Select Year");

        ui->courseBox->clear();
        ui->courseBox->addItem("Select Course");
        return;
    }

    ui->yearBox->clear();
    ui->yearBox->addItem("Select Year");

    const QStringList years = m_courseDb->yearsForMajor(major);
    for (const QString &y : years)
        ui->yearBox->addItem(y);
}

void MainWindow::onYearChanged(const QString &year)
{
    const QString major = ui->majorBox->currentText();
    if (year == "Select Year" || major == "Select Major")
    {
        ui->courseBox->clear();
        ui->courseBox->addItem("Select Course");
        return;
    }

    ui->courseBox->clear();
    ui->courseBox->addItem("Select Course");

    const QStringList courses =
        m_courseDb->coursesForMajorYear(major, year);

    for (const QString &code : courses)
        ui->courseBox->addItem(code);
}

void MainWindow::onCourseChanged(const QString &courseCode)
{
    if (courseCode == "Select Course")
        return;

    updateCourseDisplay(courseCode);
}

void MainWindow::updateCourseDisplay(const QString &courseCode)
{
    CourseInfo cinfo = m_courseDb->courseByCode(courseCode);
    if (cinfo.code.isEmpty())
        return;

    ui->courseLabel->setText("Course: " + cinfo.code + " - " + cinfo.name);

    QVector<ProfessorInfo> profs =
        m_profDb->professorsForCourse(courseCode);

    ui->profTable->setRowCount(0);
    QString summaryText;
    QString firstIsbn;

    int maxRows = qMin(5, profs.size());

    for (int i = 0; i < maxRows; ++i)
    {
        const ProfessorInfo &p = profs[i];
        ui->profTable->insertRow(i);

        ui->profTable->setItem(i, 0, new QTableWidgetItem(p.professorInfo));
        ui->profTable->setItem(i, 1, new QTableWidgetItem(p.bookTitle));
        ui->profTable->setItem(i, 2, new QTableWidgetItem(p.author));
        ui->profTable->setItem(i, 3, new QTableWidgetItem(p.isbn));

        if (i == 0)
            firstIsbn = p.isbn;

        if (!p.summary.isEmpty()) {
            summaryText += p.professorInfo + ":\n";
            summaryText += p.summary + "\n\n";
        }
    }



    if (!firstIsbn.isEmpty())
    {
        ui->isbnLabel->setText("ISBN: " + firstIsbn);

        QString imgPath = "covers/" + firstIsbn + ".jpg";
        QPixmap pix(imgPath);

        if (pix.isNull())
        {
            imgPath = "covers/" + firstIsbn + ".png";
            pix.load(imgPath);
        }

        if (!pix.isNull())
        {
            ui->coverLabel->setPixmap(
                pix.scaled(ui->coverLabel->size(),
                           Qt::KeepAspectRatio,
                           Qt::SmoothTransformation));
            ui->coverLabel->setText("");
        } else {
            ui->coverLabel->setText("No Cover");
            ui->coverLabel->setPixmap(QPixmap());
        }
    }
}

void MainWindow::onIsbnInputReturnPressed()
{
    QString isbn = ui->isbnInput->text().trimmed();
    if (isbn.isEmpty())
        return;

    ProfessorInfo pinfo = m_profDb->byIsbn(isbn);

    if (pinfo.courseCode.isEmpty())
    {
        QMessageBox::information(this, "Not Found",
                                 "No professor/book found for this ISBN.");
        return;
    }

    CourseInfo cinfo = m_courseDb->courseByCode(pinfo.courseCode);
    if (cinfo.code.isEmpty())
        return;

    int majorIdx = ui->majorBox->findText(cinfo.major);
    if (majorIdx >= 0)
        ui->majorBox->setCurrentIndex(majorIdx);

    int yearIdx = ui->yearBox->findText(cinfo.year);
    if (yearIdx >= 0)
        ui->yearBox->setCurrentIndex(yearIdx);

    int courseIdx = ui->courseBox->findText(cinfo.code);
    if (courseIdx >= 0)
        ui->courseBox->setCurrentIndex(courseIdx);

    updateCourseDisplay(cinfo.code);
}

void MainWindow::onLoginClicked()
{
    LoginDialog dlg(m_auth, this);
    connect(&dlg, &LoginDialog::loginSuccess,
            this,  &MainWindow::onLoginSuccess);

    dlg.exec();
}

void MainWindow::onLoginSuccess(const QString &username)
{
    m_currentUser = username;
    setLoggedIn(true);

    QMessageBox::information(this, "Welcome",
                             "Welcome, " + username + "!");
}

void MainWindow::onLogoutClicked()
{
    if (!m_loggedIn)
        return;

    m_currentUser.clear();
    setLoggedIn(false);

    QMessageBox::information(this, "Logout",
                             "You have logged out.");
}
