#include "../include/auth.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>

using namespace std;

bool isValidUsername(const string& username) {
    if (username.length() < 4 || username.length() > 12) return false;
    for (char c : username) {
        if (!isalnum(c)) return false; 
    }
    return true;
}

// Validasi Keamanan: Memastikan tidak ada karakter titik koma (;)
bool isSafeFromDelimiter(const string& str) {
    return str.find(';') == string::npos && !str.empty();
}


// FUNGSI UTAMA AUTH
void loadUsersDariFile(vector<User>& daftarUser) {
    ifstream file("../database/user.txt");
    if (!file.is_open()) return;

    string baris;
    daftarUser.clear();
    while (getline(file, baris)) {
        if (baris.empty()) continue;
        stringstream ss(baris);
        string u, p, r;
        getline(ss, u, ';');
        getline(ss, p, ';');
        getline(ss, r, ';');
        daftarUser.push_back({u, p, r});
    }
    file.close();
}

void simpanUserKeFile(const User& userBaru) {
    bool butuhEnterBaru = false;

    // 1. Buka file dalam mode baca (ifstream) untuk mengintip isinya
    ifstream fileIn("../database/user.txt");
    if (fileIn.is_open()) {
        fileIn.seekg(0, ios::end);
        streampos ukuranFile = fileIn.tellg();

        if (ukuranFile > 0) {
            fileIn.seekg(-1, ios::end);
            char karakterTerakhir;
            fileIn.get(karakterTerakhir);

            if (karakterTerakhir != '\n') {
                butuhEnterBaru = true;
            }
        }
        fileIn.close();
    }

    // 2. Buka file dalam mode tulis (ofstream) untuk menyimpan data
    ofstream fileOut("../database/user.txt", ios::app); 
    if (fileOut.is_open()) {
        if (butuhEnterBaru) {
            fileOut << "\n"; // Beri "Enter" otomatis sebagai pemisah
        }
        fileOut << userBaru.username << ";" << userBaru.password << ";" << userBaru.role << "\n";
        fileOut.close();
    }
}

void prosesRegister(vector<User>& daftarUser) {
    User userBaru;
    string inputRole;
    bool usernameSudahAda;

    cout << "\n==================================\n";
    cout << "        HALAMAN REGISTRASI        \n";
    cout << "==================================\n";

    // 1. Validasi Username (Alphanumeric & Length)
    while (true) {
        cout << " Masukkan Username Baru (4-12 huruf/angka): ";
        getline(cin >> ws, userBaru.username); 

        if (!isValidUsername(userBaru.username)) {
            cout << " [!] Error: Username tidak valid. Gunakan 4-12 tanpa spasi atau simbol.\n";
            continue;
        }

        usernameSudahAda = false;
        for (const auto& user : daftarUser) {
            if (user.username == userBaru.username) {
                usernameSudahAda = true;
                break;
            }
        }

        if (usernameSudahAda) {
            cout << " [!] Gagal: Username '" << userBaru.username << "' sudah digunakan.\n";
        } else {
            break;
        }
    }

    // 2. Validasi Password (Anti-Delimiter Injection)
    while (true) {
        cout << " Masukkan Password (tanpa titik koma ';'): ";
        getline(cin >> ws, userBaru.password);
        
        if (!isSafeFromDelimiter(userBaru.password)) {
            cout << " [!] Error: Password tidak boleh kosong atau mengandung titik koma (;).\n";
        } else {
            break;
        }
    }
    
    // 3. Validasi Role (Anti Infinite Loop dari input cin)
    while (true) {
        cout << " Pilih Role (1. Notaris / 2. AhliWaris): ";
        getline(cin >> ws, inputRole); 

        if (inputRole == "1") {
            userBaru.role = "Notaris";
            break;
        } else if (inputRole == "2") {
            userBaru.role = "AhliWaris";
            break;
        } else {
            cout << " [!] Error: Pilihan tidak valid! Ketik angka 1 atau 2.\n";
        }
    }

    daftarUser.push_back(userBaru);
    simpanUserKeFile(userBaru);
    cout << "\n [v] Registrasi Berhasil! Akun Anda siap digunakan untuk Login.\n";
}

bool prosesLogin(const vector<User>& daftarUser, User& userAktif) {
    string inputUsername, inputPassword;
    
    cout << "\n==================================\n";
    cout << "          HALAMAN LOGIN           \n";
    cout << "==================================\n";
    
    cout << "   USERNAME : "; 
    getline(cin >> ws, inputUsername);
    
    cout << "   PASSWORD : "; 
    getline(cin >> ws, inputPassword);
    
    cout << "==================================\n";

    for (const auto& user : daftarUser) {
        if (user.username == inputUsername && user.password == inputPassword) {
            userAktif = user; 
            return true;
        }
    }
    return false; 
}