# RoyaleDeliveryClient 🚚
The RoyaleDeliveryClient is a Qt-based application designed to provide a seamless and efficient way to manage deliveries. It offers a range of features, including user authentication, email sending, and remote PI server connection. The application is built using the Qt framework, which provides a comprehensive set of libraries and tools for building cross-platform applications.

## 🚀 Features
- User authentication and authorization
- Email sending using SMTP connection
- Remote PI server connection for command execution
- User-friendly interface for easy navigation and management
- Support for multiple platforms, including Android and iOS

## 🛠️ Tech Stack
- Qt framework (version 5 or 6)
- C++ compiler (version 17)
- QTcpSocket and QSslConfiguration for secure network communication
- QCryptographicHash and QDataStream for data encryption and serialization
- QString and QDataStream for string manipulation and data storage

## 📦 Installation
To install the RoyaleDeliveryClient, follow these steps:
1. Clone the repository using Git: `git clone https://github.com/your-repo/royale-delivery-client.git`
2. Navigate to the project directory: `cd royale-delivery-client`
3. Install the required dependencies: `sudo apt-get install qt5-default` (for Ubuntu-based systems)
4. Build the project using CMake: `cmake .` and `make`
5. Run the application: `./RoyaleDeliveryClient`

## 💻 Usage
To use the RoyaleDeliveryClient, follow these steps:
1. Launch the application: `./RoyaleDeliveryClient`
2. Create an account or log in to an existing one
3. Navigate to the email sending feature and compose an email
4. Connect to the remote PI server and execute commands

## 📂 Project Structure
```markdown
RoyaleDeliveryClient/
├── CMakeLists.txt
├── headers/
│   ├── globals.h
│   ├── net/
│   │   ├── commands.h
│   │   ├── remotepi.h
│   │   └── smtpconnection.h
│   └── windows/
│       ├── loginwindow.h
│       ├── mainwindow.h
│       └── signupwindow.h
├── source/
│   ├── main.cpp
│   ├── net/
│   │   ├── smtpconnection.cpp
│   │   └── remotepi.cpp
│   └── windows/
│       ├── loginwindow.cpp
│       ├── mainwindow.cpp
│       └── signupwindow.cpp
└── README.md
```

## 📸 Screenshots
