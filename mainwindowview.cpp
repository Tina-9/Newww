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

MainWindowView::MainWindowView(QWidget *parent)
    : QWidget(parent)
{
    // 整体背景渐变（和你图片很像）
    setStyleSheet(
        "MainWindowView {"
        "  background: qlineargradient("
        "      x1:0, y1:0, x2:1, y2:1,"
        "      stop:0 #F2DB7A,"
        "      stop:1 #FFFFFF);"
        "}"
        );

    // ========= 顶部按钮 =========
    btnLogin  = new QPushButton("Login");
    btnSignUp = new QPushButton("Sign Up");
    btnLogout = new QPushButton("Logout");

    auto *topBtnLayout = new QHBoxLayout;
    topBtnLayout->addStretch();
    topBtnLayout->addWidget(btnLogin);
    topBtnLayout->addWidget(btnSignUp);
    topBtnLayout->addWidget(btnLogout);

    // ========= 左侧区域 =========

    auto *lblIsbnInput = new QLabel("ISBN Input:");
    isbnInput = new QLineEdit;
    isbnInput->setPlaceholderText("Type ISBN and press Enter...");

    auto *lblMajor  = new QLabel("Major:");
    auto *lblYear   = new QLabel("Year:");
    auto *lblCourse = new QLabel("Course:");

    majorBox  = new QComboBox;
    yearBox   = new QComboBox;
    courseBox = new QComboBox;

    majorBox->addItem("Select Major");
    yearBox->addItem("Select Year");
    courseBox->addItem("Select Course");

    auto *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topBtnLayout);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(lblIsbnInput);
    leftLayout->addWidget(isbnInput);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(lblMajor);
    leftLayout->addWidget(majorBox);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(lblYear);
    leftLayout->addWidget(yearBox);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(lblCourse);
    leftLayout->addWidget(courseBox);
    leftLayout->addStretch();

    // ========= 右侧顶部：课程 + 封面 + ISBN 显示 =========

    courseLabel = new QLabel("Course: -");
    QFont f = courseLabel->font();
    f.setPointSize(16);
    f.setBold(true);
    courseLabel->setFont(f);

    isbnLabel = new QLabel("ISBN: -");

    auto *courseHeaderLayout = new QHBoxLayout;
    courseHeaderLayout->addWidget(courseLabel);
    courseHeaderLayout->addStretch();
    courseHeaderLayout->addWidget(isbnLabel);

    coverLabel = new QLabel("No Cover");
    coverLabel->setFixedSize(350, 220);
    coverLabel->setAlignment(Qt::AlignCenter);
    coverLabel->setStyleSheet("background:white; border:1px solid #CFAE3D;");

    auto *coverLayout = new QHBoxLayout;
    coverLayout->addStretch();
    coverLayout->addWidget(coverLabel);
    coverLayout->addStretch();

    // ========= 表格 =========
    profTable = new QTableWidget;
    profTable->setColumnCount(4);
    QStringList headers;
    headers << "Professor" << "Book Title" << "Author" << "ISBN";
    profTable->setHorizontalHeaderLabels(headers);
    profTable->horizontalHeader()->setStretchLastSection(true);
    profTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    profTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // ========= Summary =========
    summaryEdit = new QTextEdit;
    summaryEdit->setPlaceholderText("Summary about this course, professors, and books...");
    summaryEdit->setReadOnly(true);

    auto *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(courseHeaderLayout);
    rightLayout->addLayout(coverLayout);
    rightLayout->addWidget(profTable, 3);
    rightLayout->addWidget(summaryEdit, 2);

    // ========= 主布局 =========
    auto *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 3);
    setLayout(mainLayout);
}
