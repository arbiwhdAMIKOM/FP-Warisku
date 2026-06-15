#include <iostream>
#include <vector>
#include "../include/models.h"
#include "../include/auth.h"

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void jedaLayar() {
    cout << "\nTekan \"Enter\" untuk melanjutkan...";
    cin.get(); 
}

void menuCRUDRahmat() {
    cout << "\n[MENU CRUD AKTIF] - Rahmat kerja di sini...\n";
}

void menuKalkulatorLulut() {
    cout << "\n[MENU KALKULATOR AKTIF] - Lulut kerja di sini...\n";
}

int main() {
    vector<User> databaseUser;
    
    loadUsersDariFile(databaseUser);

    while (true) {
        User userLogedIn;
        bool isSessionActive = false;
        string menuAwalStr; 

        do {
            clearScreen();
            cout << "======================================\n";
            cout << "       SELAMAT DATANG DI WARISKU       \n";
            cout << "======================================\n";
            cout << "\n1. Login\n2. Register\n0. Keluar Aplikasi\nPilih opsi: ";
            getline(cin >> ws, menuAwalStr); 

            if (menuAwalStr == "1") {
                isSessionActive = prosesLogin(databaseUser, userLogedIn);
                if (!isSessionActive) {
                    cout << "Login Gagal! Username atau Password salah.\n";
                }
            } else if (menuAwalStr == "2") {
                prosesRegister(databaseUser);
                loadUsersDariFile(databaseUser); 
            } else if (menuAwalStr == "0") {
                cout << "Keluar dari aplikasi. Sampai jumpa!\n";
                return 0; 
            } else {
                cout << " [!] Error: Pilihan tidak valid!\n";
            }
        } while (!isSessionActive);

        string pilihanMenuStr; 
        bool isLogout = false;

        do {
            clearScreen();
            cout << "Login Sukses! Selamat Datang, " << userLogedIn.username << " (" << userLogedIn.role << ")\n";
            jedaLayar();
            clearScreen();
            cout << "\n============ MENU UTAMA ============\n";
            if (userLogedIn.role == "Notaris") {
                cout << "1. Kelola Data Aset & Keluarga\n";
                cout << "2. Hitung & Export Pembagian Waris\n";
            } else if (userLogedIn.role == "AhliWaris") {
                cout << "1. Lihat Pembagian Waris & Status Klaim\n";
            }
            cout << "9. Logout (Kembali ke Menu Awal)\n";
            cout << "0. Keluar Aplikasi\n";
            cout << "Pilih Menu: "; 
            getline(cin >> ws, pilihanMenuStr);

            if (pilihanMenuStr == "1") {
                if (userLogedIn.role == "Notaris") {
                    menuCRUDRahmat(); 
                } else {
                    cout << "\n[Fitur Ahli Waris] Menampilkan hasil porsi waris...\n"; 
                }
            } else if (pilihanMenuStr == "2" && userLogedIn.role == "Notaris") {
                menuKalkulatorLulut(); 
            } else if (pilihanMenuStr == "9") {
                cout << "\nLogout berhasil. Kembali ke Menu Awal...\n";
                isLogout = true; 
                jedaLayar();
            } else if (pilihanMenuStr == "0") {
                cout << "\nKeluar dari aplikasi. Sampai jumpa!\n";
                return 0; 
            } else {
                cout << " [!] Error: Pilihan tidak valid!\n";
            }

        } while (!isLogout); 
    }

    return 0;
}