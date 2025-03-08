#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "DatabaseHeader.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ptrRegister = new Register();

}

MainWindow::~MainWindow()
{
    delete ptrRegister;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ptrRegister->show();
}


void MainWindow::on_pushButton_2_clicked()
{
    // Check if a database connection already exists
    QSqlDatabase db;
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        // Create a new SQLite database connection
        db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
        db.setDatabaseName("User_registration_system.db");
    }
    // Attempt to open the database
    if (!db.isOpen()) {
        if (!db.open()) {
            qDebug() << "Error: Database connection failed:" << db.lastError().text();
            return;
        }
    }

   // Unlock the database (if needed)
        QSqlQuery unlockQuery(db);
        if (!unlockQuery.exec("PRAGMA journal_mode=WAL;")) {
            qDebug() << "Error: Couldn't unlock database:" << unlockQuery.lastError().text();
        } else {
            qDebug() << "Database unlocked successfully.";
        }

        // Prepare a query for user login
        QSqlQuery query(db);

        db.transaction();  // Start transaction
        // Collect user-provided email and password
        QString email = ui->emailInput->text();
        QString password = ui->passwordInput->text();

        // Hash the entered password before querying the database for security
        QString hashedPassword = hashPassword(password);

        // Prepare the SQL query to verify credentials
        query.prepare("SELECT * FROM users WHERE Email = :email AND Password = :hashedPassword");
        query.bindValue(":email", email);
        query.bindValue(":hashedPassword", hashedPassword);

        // Execute the query and check for results
        if (query.exec() && query.next()) {
            //Login successfull
            QString username = query.value("Username").toString();
            QMessageBox::information(this, "Login Successful", "Welcome, " + username + "!");
            MainPage *mainpage = new MainPage();
            mainpage->show();


        } else {
            // Login Failed
            QMessageBox::warning(this, "Login Failed", "Invalid email or password.");
        }
    // Close the database connection when finished
    db.close();
}

