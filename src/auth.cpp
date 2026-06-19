#include "../include/auth.h"
#include "../include/ui.h" // Panggil engine UI
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

void clearScr() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void jeda() {
    cout << "\nTekan \"Enter\" untuk melanjutkan...";
    cin.get(); 
}

bool isValidUsername(const string& username) {
    if (username.length() < 4 || username.length() > 12) return false;
    for (char c : username) {
        if (!isalnum(c)) return false; 
    }
    return true;
}

bool isSafeFromDelimiter(const string& str) {
    return str.find(';') == string::npos && !str.empty();
}

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
    ifstream fileIn("../database/user.txt");
    if (fileIn.is_open()) {
        fileIn.seekg(0, ios::end);
        streampos ukuranFile = fileIn.tellg();
        if (ukuranFile > 0) {
            fileIn.seekg(-1, ios::end);
            char karakterTerakhir;
            fileIn.get(karakterTerakhir);
            if (karakterTerakhir != '\n') butuhEnterBaru = true;
        }
        fileIn.close();
    }

    ofstream fileOut("../database/user.txt", ios::app); 
    if (fileOut.is_open()) {
        if (butuhEnterBaru) fileOut << "\n"; 
        fileOut << userBaru.username << ";" << userBaru.password << ";" << userBaru.role << "\n";
        fileOut.close();
    }
}

void prosesRegister(vector<User>& daftarUser) {
    User userBaru;
    bool usernameSudahAda;

    clearScr();
    cout << "\n==================================\n";
    cout << "        HALAMAN REGISTRASI        \n";
    cout << "==================================\n";

    while (true) {
        cout << " Masukkan Username Baru (4-12 huruf/angka): ";
        getline(cin >> ws, userBaru.username); 

        if (!isValidUsername(userBaru.username)) {
            cout << " [!] Error: Username tidak valid.\n";
            continue;
        }
        usernameSudahAda = false;
        for (const auto& user : daftarUser) {
            if (user.username == userBaru.username) {
                usernameSudahAda = true;
                break;
            }
        }
        if (usernameSudahAda) cout << " [!] Gagal: Username sudah digunakan.\n";
        else break;
    }

    while (true) {
        cout << " Masukkan Password (tanpa titik koma ';'): ";
        getline(cin >> ws, userBaru.password);
        if (!isSafeFromDelimiter(userBaru.password)) cout << " [!] Error: Password tidak valid.\n";
        else break;
    }
    
    // UBAHAN: Pakai tombol panah untuk milih Role
    vector<string> opsiRole = {"Notaris", "AhliWaris"};
    int pilRole = menuInteraktif("Pilih Role Akun Anda", opsiRole);
    userBaru.role = (pilRole == 0) ? "Notaris" : "AhliWaris";

    daftarUser.push_back(userBaru);
    simpanUserKeFile(userBaru);
    cout << "\n [v] Registrasi Berhasil! Silakan Login.\n";
}

bool prosesLogin(const vector<User>& daftarUser, User& userAktif) {
    string inputUsername, inputPassword;
    clearScr();
    cout << "\n==================================\n";
    cout << "          HALAMAN LOGIN           \n";
    cout << "==================================\n";
    cout << "   USERNAME : "; getline(cin >> ws, inputUsername);
    cout << "   PASSWORD : "; getline(cin >> ws, inputPassword);
    cout << "==================================\n";

    for (const auto& user : daftarUser) {
        if (user.username == inputUsername && user.password == inputPassword) {
            userAktif = user; 
            return true;
        }
    }
    return false; 
}