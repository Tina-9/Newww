#include "mainwindowview.h"

#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QTableWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QFile>
#include <QDebug>


MainWindowView::MainWindowView(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(
        "background: qradialgradient("
        "    cx:0.5, cy:0.3, radius:0.85,"
        "    fx:0.5, fy:0.3,"
        "    stop:0   #FFFDEB,"
        "    stop:0.5 #FFE7B8,"
        "    stop:1   #FFCFA5"
        ");"
        );


    // top button
    btnLogin  = new QPushButton("Login");
    btnSignUp = new QPushButton("Sign Up");
    btnLogout = new QPushButton("Logout");

    auto *topBtnLayout = new QHBoxLayout;
    topBtnLayout->addStretch();
    topBtnLayout->addWidget(btnLogin);
    topBtnLayout->addWidget(btnSignUp);
    topBtnLayout->addWidget(btnLogout);

    // Left

    isbnInput = new QLineEdit;
    isbnInput->setPlaceholderText("Type ISBN and press Enter...");
    isbnInput->setStyleSheet(
        "QLineEdit {"
        "   background: #FFF9E8;"
        "   border: 2px solid #F6D78E;"
        "   border-radius: 8px;"
        "   padding: 4px 8px;"
        "   color: #6B4E2E;"
        "}"
        );

    majorBox  = new QComboBox;
    yearBox   = new QComboBox;
    courseBox = new QComboBox;

    majorBox->addItem("Select Major");
    majorBox->setStyleSheet(
        "QComboBox {"
        "   background: #FFF9E8;"
        "   border: 2px solid #F6D78E;"
        "   border-radius: 8px;"
        "   padding: 4px 8px;"
        "   color: #6B4E2E;"
        "}"
        );
    yearBox->addItem("Select Year");
    yearBox->setStyleSheet(majorBox->styleSheet());

    courseBox->addItem("Select Course");
    courseBox->setStyleSheet(majorBox->styleSheet());

    QFrame *leftPanel = new QFrame;
    leftPanel->setStyleSheet(
        "background: rgba(255,255,255,0.86);"
        "border: 2px solid #F6D78E;"
        "border-radius: 12px;"
        );

    auto *leftLayout = new QVBoxLayout;
    leftPanel->setLayout(leftLayout);

    leftLayout->addLayout(topBtnLayout);
    leftLayout->addSpacing(250);
    leftLayout->addWidget(isbnInput);

    leftLayout->addSpacing(10);
    leftLayout->addWidget(majorBox);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(yearBox);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(courseBox);
    leftLayout->addStretch();

    // right top: course + cover + ISBN

    courseLabel = new QLabel("Course: ");
    QFont f = courseLabel->font();
    f.setPointSize(16);
    f.setBold(true);
    courseLabel->setFont(f);

    isbnLabel = new QLabel("ISBN: ");
    isbnLabel->setVisible(true);
    titleLabel  = new QLabel("Title: ");
    authorLabel = new QLabel("Author: ");

    auto *courseHeaderLayout = new QHBoxLayout;
    courseHeaderLayout->addWidget(courseLabel);
    courseLabel->setStyleSheet("color: #6B4E2E;");
    isbnLabel->setStyleSheet("color: #6B4E2E;");
    titleLabel->setStyleSheet("color: #6B4E2E;");
    authorLabel->setStyleSheet("color: #6B4E2E;");

    courseHeaderLayout->addStretch();
    courseHeaderLayout->addWidget(isbnLabel);



    QVBoxLayout *bookInfoLayout = new QVBoxLayout;
    bookInfoLayout->addWidget(titleLabel);
    bookInfoLayout->addWidget(authorLabel);
    bookInfoLayout->addStretch();

    coverLabel = new QLabel("No Cover");
    coverLabel->setFixedSize(350, 220);
    coverLabel->setAlignment(Qt::AlignCenter);
    coverLabel->setStyleSheet(
        "background: #FFFFFF;"
        "border: 2px solid #F6D78E;"
        "border-radius: 8px;"
        );

    auto *coverLayout = new QHBoxLayout;
    coverLayout->addWidget(coverLabel);
    coverLayout->addLayout(bookInfoLayout);

    // table



    profTable = new QTableWidget;

    profTable->setStyleSheet(
        "QHeaderView::section {"
        "    background: #FFE7B8;"
        "    color: #6B4E2E;"
        "    border: 1px solid #F6D78E;"
        "    font-weight: bold;"
        "}"
        "QTableWidget {"
        "    background: rgba(255,255,255,0.85);"
        "    border: 2px solid #F6D78E;"
        "    border-radius: 8px;"
        "    gridline-color: #F6D78E;"
        "}"
        "QTableWidget::item:selected {"
        "    background: #FFF4C9;"
        "    color: #6B4E2E;"
        "}"
        );

    profTable->setColumnCount(4);
    profTable->verticalHeader()->setFixedWidth(25);
    profTable->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    profTable->verticalHeader()->setStyleSheet("font-size: 10pt;");

    QStringList headers;
    headers << "Professor" << "Book Title" << "Author" << "All Courses";
    profTable->setHorizontalHeaderLabels(headers);
    profTable->setColumnWidth(0, 120);
    profTable->setColumnWidth(1, 210);
    profTable->setColumnWidth(2, 150);
    profTable->setColumnWidth(3, 300);
    profTable->horizontalHeader()->setStretchLastSection(false);
    profTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    profTable->setSelectionBehavior(QAbstractItemView::SelectRows);


    // Summary
    summaryEdit = new QTextEdit;
    summaryEdit->setStyleSheet(
        "QTextEdit {"
        "   background: #FFFDF3;"
        "   border: 2px solid #F6D78E;"
        "   border-radius: 10px;"
        "   padding: 8px;"
        "   color: #6B4E2E;"
        "}"
        );
    summaryEdit->setPlaceholderText("Summary about this course, professors, and books...");
    summaryEdit->setReadOnly(true);

    auto *rightLayout = new QVBoxLayout;
    QFrame *rightPanel = new QFrame;
    rightPanel->setStyleSheet(
        "background: rgba(255,255,255,0.80);"
        "border: 2px solid #F6D78E;"
        "border-radius: 12px;"
        );
    rightPanel->setLayout(rightLayout);

    rightLayout->addLayout(courseHeaderLayout);
    rightLayout->addLayout(coverLayout);
    rightLayout->addWidget(profTable, 3);
    rightLayout->addWidget(summaryEdit, 2);

    // main layout
    auto *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(leftPanel, 1);
    mainLayout->addWidget(rightPanel, 3);
    setLayout(mainLayout);


}
