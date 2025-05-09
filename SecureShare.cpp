//____________________________________________________
//---- SecureShare : Role-Based File Protection ----


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath> 

using namespace std;

//to store user information
struct User {
    string username;
    string password;
    string role; // admin or user
};

vector<User> users;

// Initialize with a default admin user
void initialize_users() {
    users.push_back((User){"admin", "admin123", "admin"});
}

// Function to authenticate a user
int authenticate() {
    string username, password;

    cout << "Enter username (or type 'exit' to quit): ";
    cin >> username;
    if (username == "exit") {
        return -2;
    }

    cout << "Enter password: ";
    cin >> password;

    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].username == username && users[i].password == password) {
            return i; 
        }
    }
    cout << "Invalid credentials. Please try again." << endl;
    return -1;
}

// Function for admins to add a new user
void add_user() {
    string username, password, role;
    cout << "Enter new username: ";
    cin >> username;
    cout << "Enter new password: ";
    cin >> password;
    cout << "Enter role (admin/user): ";
    cin >> role;

    if (role != "admin" && role != "user") {
        cout << "Invalid role. User not added." << endl;
        return;
    }

    users.push_back((User){username, password, role});
    cout << "User added successfully!" << endl;
}
// 2FA: Generate a random 6-digit code
int generate_2fa_code() {
    srand(time(0)); 
    return rand() % 900000 + 100000; // Generate a 6-digit number
}

struct RSAKey {
    long long n;
    long long key; 
};

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to calculate modular exponentiation (base^exp % mod)
long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1; // exp = exp / 2
        base = (base * base) % mod;
    }
    return result;
}

// Function to generate RSA keys
void generate_rsa_keys(RSAKey &public_key, RSAKey &private_key) {
    long long p = 61; // First prime number
    long long q = 53; // Second prime number
    long long n = p * q;
    long long phi = (p - 1) * (q - 1);

    long long e = 3; 
    while (gcd(e, phi) != 1) {
        e += 2; 
    }

    long long d = 1; 
    while ((d * e) % phi != 1) {
        d++;
    }

    public_key = (RSAKey){n, e};
    private_key = (RSAKey){n, d};
}

long long rsa_encrypt_char(char ch, const RSAKey &key) {
    return mod_exp(static_cast<long long>(ch), key.key, key.n);
}

char rsa_decrypt_char(long long enc_ch, const RSAKey &key) {
    return static_cast<char>(mod_exp(enc_ch, key.key, key.n));
}

// Function to encrypt a file using RSA
void encrypt_file(const string &filename, const RSAKey &public_key) {
    ifstream infile(filename.c_str());
    ofstream outfile((filename + ".enc").c_str());
    if (!infile || !outfile) {
        cout << "Error: Unable to open file for encryption!" << endl;
        return;
    }

    char ch;
    while (infile.get(ch)) {
        long long enc_ch = rsa_encrypt_char(ch, public_key);
        outfile << enc_ch << ' '; 
    }
    infile.close();
    outfile.close();

    cout << "\nFile encrypted successfully! Output: " << filename << ".enc" << endl;
}

// Function to decrypt a file using RSA
void decrypt_file(const string &filename, const RSAKey &private_key) {
    ifstream infile(filename.c_str());
    ofstream outfile((filename + ".dec").c_str());
    if (!infile || !outfile) {
        cout << "Error: Unable to open file for decryption!" << endl;
        return;
    }

    long long enc_ch;
    while (infile >> enc_ch) { 
        char dec_ch = rsa_decrypt_char(enc_ch, private_key);
        outfile.put(dec_ch); 
    }
    infile.close();
    outfile.close();

    cout << "\nFile decrypted successfully! Output: " << filename << ".dec" << endl;
}



// Function for admins to modify file content
void modify_file(const string &filename) {
    ofstream file(filename.c_str(), ios::app); // Open file in append mode
    if (!file) {
        cout << "Error: Unable to open file for modification!" << endl;
        return;
    }

    cout << "Enter content to append to the file: ";
    string content;
    cin.ignore(); 
    getline(cin, content);

    file << content << endl; 
    file.close();

    cout << "Content added successfully!" << endl;
}

int main() {
    cout << "=== Secure File Sharing with Role-Based Access Control ===" << endl;

    initialize_users();

    RSAKey public_key, private_key;
    generate_rsa_keys(public_key, private_key);

    while (true) {
        int user_index = -1;
        while (user_index == -1) {
            user_index = authenticate();
            if (user_index == -2) { 
                cout << "Exiting program. Goodbye!" << endl;
                return 0;
            }
        }

        User current_user = users[user_index];

        int correct_2fa_code = generate_2fa_code();
        cout << "Enter the 2FA code sent to your device: " << correct_2fa_code << endl;
        int entered_code;
        cin >> entered_code;
        if (entered_code != correct_2fa_code) {
            cout << "Invalid 2FA code. Access denied." << endl;
            continue; // Return to login screen
        }
        cout << "2FA Successful! Logged in as " << current_user.username << " (" << current_user.role << ")." << endl;

        string filename;
        bool logged_in = true;

        while (logged_in) {
            cout << "\n=== Main Menu ===" << endl;
            if (current_user.role == "admin") {
                cout << "1. Add a new user" << endl;
                cout << "2. Modify file content" << endl;
                cout << "3. Encrypt a file" << endl;
                cout << "4. Decrypt a file" << endl;
                cout << "5. Logout" << endl;

                int choice;
                cin >> choice;

                if (choice == 1) {
                    add_user();
                } else if (choice == 2) {
                    cout << "Enter the name of the file to modify: ";
                    cin >> filename;
                    modify_file(filename);
                } else if (choice == 3) {
                    cout << "Enter the name of the file to encrypt: ";
                    cin >> filename;
                    encrypt_file(filename, public_key);
                } else if (choice == 4) {
                    cout << "Enter the name of the file to decrypt: ";
                    cin >> filename;
                    decrypt_file(filename, private_key);
                } else if (choice == 5) {
                    cout << "Logging out..." << endl;
                    logged_in = false;
                } else {
                    cout << "Invalid choice. Try again." << endl;
                }
            } else if (current_user.role == "user") {
                cout << "1. Encrypt a file" << endl;
                cout << "2. Decrypt a file" << endl;
                cout << "3. Logout" << endl;

                int choice;
                cin >> choice;

                if (choice == 1) {
                    cout << "Enter the name of the file to encrypt: ";
                    cin >> filename;
                    encrypt_file(filename, public_key);
                } else if (choice == 2) {
                    cout << "Enter the name of the file to decrypt: ";
                    cin >> filename;
                    decrypt_file(filename, private_key);
                } else if (choice == 3) {
                    cout << "Logging out..." << endl;
                    logged_in = false;
                } else {
                    cout << "Invalid choice. Try again." << endl;
                }
            }
        }
    }

    return 0;
}

