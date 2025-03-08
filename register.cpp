#include "register.h"
#include "ui_register.h"
#include <QDebug>
#include <QFile>
#include <sys/stat.h>
#include <QMessageBox>
#include "utils.h"
#include <QRegularExpression>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}

// Function to validate email format using regular expression
bool isValidEmail(const QString &email) {
    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex.match(email).hasMatch();
}

// Function to validate password strength
bool isStrongPassword(const QString &password) {
    // Password must be at least 8 characters, contain uppercase, lowercase, digit, and special character
    QRegularExpression regex(R"((?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[@$!%*?&])[A-Za-z\d@$!%*?&]{8,})");
    return regex.match(password).hasMatch();
}

// Registration button click handler
void Register::on_pushButton_clicked()
{
        // Collect and trim user inputs
        QString userName = ui->txtUserName->text().trimmed();
        QString email = ui->txtEmail->text().trimmed();
        QString password = ui->txtPassword->text().trimmed();
        QString confirmPassword = ui->txtConfPassword->text().trimmed();


        // Validate input fields
        if (userName.isEmpty() || email.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "All fields must be filled!");
            return;
        }

        // Ensure password meets strength criteria
        if (!isStrongPassword(password)) {
            QMessageBox::warning(this, "Weak Password", "Password must be at least 8 characters and include uppercase, lowercase, digit, and special character.");
            return;
         }
        // Check for valid email format
        if (!isValidEmail(email)) {
            QMessageBox::warning(this, "Invalid Email", "Please enter a valid email address!");
            return;
        }

        // Ensure password confirmation matches
        if (password != confirmPassword) {
            QMessageBox::warning(this, "Password Mismatch", "Passwords do not match!");
            return;
        }

        // Database connection setup
        QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
        if (!db.isValid()) {
            db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
            db.setDatabaseName("User_registration_system.db");
        }

        // Database connection check
        if (!db.open()) {
            QMessageBox::critical(this, "Database Error", "Failed to connect to the database!");
            return;
        }

        // Create the database and Users table if they don't exist
        QSqlQuery createTableQuery(db);
        if (!createTableQuery.exec(R"(
            CREATE TABLE IF NOT EXISTS Users (
                Username TEXT PRIMARY KEY NOT NULL,
                Email TEXT UNIQUE NOT NULL,
                Password TEXT NOT NULL
            )
        )")) {
            QMessageBox::critical(this, "Database Error", "Failed to create Users table: " + createTableQuery.lastError().text());
            db.close();
            return;
        } else {
            qDebug() << "Users table created successfully.";
        }

        // Enable WAL mode for improved performance
        QSqlQuery unlockQuery(db);
        if (!unlockQuery.exec("PRAGMA journal_mode=WAL;")) {
            QMessageBox::critical(this, "Error", "Couldn't set WAL mode: " + unlockQuery.lastError().text());
            db.close();
            return;
        }
        /*
         * for debug purpose
         * else {
            qDebug() << "Database set to WAL mode successfully.";
        }*/

        // Hash the password before storing it for security
        QString hashedPassword = hashPassword(password);


        // Check if Username or Email already exists
        // Insert new user data into the database
        QSqlQuery query(db);
        query.prepare("INSERT INTO Users (Username, Email, Password) VALUES (?, ?, ?)");
        query.addBindValue(userName);
        query.addBindValue(email);
        query.addBindValue(hashedPassword);

        // Execute the insertion query and handle errors
        if (!query.exec()) {
            qDebug() << "Error: Couldn't insert data:" << query.lastError().text();
            QMessageBox::critical(this, "Error", "Registration failed: " + query.lastError().text());
            db.rollback(); // Rollback transaction if failed
            db.close();
            return;
        } else {
            QMessageBox::information(this, "Success", "User registered successfully!");
            db.commit(); // Commit transaction if success
        }

        // Close the database connection
        db.close();

    }




