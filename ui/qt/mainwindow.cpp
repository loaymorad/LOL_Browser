#include "mainwindow.h"
#include <QCoreApplication>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Logo
    logoLabel = new QLabel(this);
    QPixmap logo("logo/logo_nobg.png");
    if (!logo.isNull()) {
        logoLabel->setPixmap(logo.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        logoLabel->setAlignment(Qt::AlignCenter);
    } else {
        logoLabel->setText("Logo not found");
    }
    layout->addWidget(logoLabel);

    // URL Input Setup
    urlInput = new QLineEdit(this);
    urlInput->setPlaceholderText("Enter URL (e.g., http://example.com)");
    layout->addWidget(urlInput);

    // Go Button Setup
    goButton = new QPushButton("Go", this);
    layout->addWidget(goButton);

    // Display Area Setup
    displayArea = new QTextEdit(this);
    displayArea->setReadOnly(true);
    layout->addWidget(displayArea);

    resize(800, 600);

    connect(goButton, &QPushButton::clicked, this, &MainWindow::handleButton);
    connect(urlInput, &QLineEdit::returnPressed, this, &MainWindow::handleButton);
}

MainWindow::~MainWindow()
{
}

void MainWindow::handleButton()
{
    std::string url = urlInput->text().toStdString();
    if (url.empty()) return;

    displayArea->setText("Loading...");
    
    // Process events to show "Loading..."
    QCoreApplication::processEvents();

    // our entry
    std::string result = browser.fetch(url);
    displayArea->setText(QString::fromStdString(result));
}
