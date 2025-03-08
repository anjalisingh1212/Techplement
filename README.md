# Techplement
A Qt C++ Login and Registration System with SQLite integration, featuring secure password hashing, email validation, and strong password enforcement.

## Features
✅ User Registration with data validation  
✅ Strong password enforcement (minimum 8 characters, uppercase, lowercase, digit, and special character)  
✅ Email format validation  
✅ Secure password storage with hashing  
✅ SQLite database integration with Write-Ahead Logging (WAL) mode for improved performance  
✅ Transaction handling to ensure data integrity  

## Requirements
- Qt Creator (recommended for development)
- Qt Framework
- SQLite (integrated within Qt)

## Setup Instructions
1. Clone this repository or download the source code.
2. Open the `.pro` or `.CMakeLists.txt` file in **Qt Creator**.
3. Build and run the project.

## Usage
1. Launch the application.
2. For registration:
   - Fill in the **Username**, **Email**, **Password**, and **Confirm Password** fields.
   - Passwords must meet strength requirements.
3. For login:
   - Enter your registered **Email** and **Password**.
   - On successful login, a welcome message will be displayed.
   
## Database Details
- The SQLite database file is named **`User_registration_system.db`**.
- Data is stored in a table called **`Users`** with the following structure:
  - `Username` (TEXT PRIMARY KEY)
  - `Email` (TEXT - UNIQUE)
  - `Password` (TEXT - Hashed for security)

## Error Handling
- Provides clear error messages for:
  - Invalid email format
  - Weak password
  - Password mismatch
  - Database connection issues

## Future Improvements
- Add password recovery functionality
- Implement user role management
- Improve UI/UX design with enhanced styling
