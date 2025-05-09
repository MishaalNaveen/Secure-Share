# 🔐 SecureShare

SecureShare is a command-line C++ application for role-based file access and protection. It integrates **RSA encryption** and a **custom Two-Factor Authentication (2FA)** mechanism to enable secure file sharing across user roles (`admin` and `user`).

---

## 🚀 Features

- 🧑‍💼 **Role-Based Access**: Admins can add users, modify files, and perform encryption/decryption; users can only encrypt/decrypt.
- 🔐 **RSA Encryption**: Files are encrypted at character-level using RSA with hardcoded primes.
- 📱 **Two-Factor Authentication (2FA)**: Custom 6-digit verification code required on each login.
- 📁 **File Modification**: Admins can append text to files securely.

---

## 🛠️ Technologies Used

- C++ (Standard Library)
- File I/O
- Basic RSA (Mathematical Implementation)
- Random 2FA Code Generation
- Role Management via Structs

---
## 👤 User Roles

### Admin (default)
- **Username:** `admin`  
- **Password:** `admin123`  
- **Abilities:** Add users, encrypt/decrypt files, modify content

### User
- **Created by admin**
- **Abilities:** Encrypt and decrypt only

---

## 📂 How to Compile and Run

```bash
g++ SecureShare.cpp -o SecureShare
./SecureShare
```

---

## 💬 Sample Session

```text
Enter username: admin  
Enter password: admin123  
Enter the 2FA code sent to your device: 385274  
2FA Successful! Logged in as admin (admin).

=== Main Menu ===
1. Add a new user  
2. Modify file content  
3. Encrypt a file  
4. Decrypt a file  
5. Logout
```

---

## 📦 File Structure

```
SecureShare/
├── SecureShare.cpp
└── README.md
```

---

## 🧠 Concepts Demonstrated

- Public-Key Cryptography (RSA)
- Role-based security controls
- Command-line interface and user input handling
- Modular arithmetic and file encryption
- Session-level 2FA logic (non-persistent)

---

## 📜 License

This project is open source and available under the [MIT License](https://choosealicense.com/licenses/mit/).

---

👨‍💻 Developed by [Mishaal Naveen](https://linkedin.com/in/mishaal-n)
