#include <iostream>
#include <vector>
#include "../include/models.h"
#include "../include/auth.h"

using namespace std;

void menuCRUDRahmat() {
    cout << "\n[MENU CRUD AKTIF] - Rahmat kerja di sini...\n";
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
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
        
        clearScreen();

        cout << "======================================\n";
        cout << "       SELAMAT DATANG DI WARISKU       \n";
        cout << "======================================\n";

        do {
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

        cout << "\nLogin Sukses! Selamat Datang, " << userLogedIn.username << " (" << userLogedIn.role << ")\n";
        
        string pilihanMenuStr; 
        bool isLogout = false;
        clearScreen();
        do {
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
                    menuCRUDRahmat(); // Rahmat: Ganti baris ini dengan fungsi CRUD mu
                } else {
                    cout << "\n[Fitur Ahli Waris] Menampilkan hasil porsi waris...\n"; // Lulut: Ganti baris ini dengan fungsi cetak/read mu
                }
            } else if (pilihanMenuStr == "2" && userLogedIn.role == "Notaris") {
                menuKalkulatorLulut(); // Lulut: Ganti baris ini dengan fungsi kalkulatormu
            } else if (pilihanMenuStr == "9") {
                cout << "\nLogout berhasil. Kembali ke Menu Awal...\n";
                isLogout = true; 
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