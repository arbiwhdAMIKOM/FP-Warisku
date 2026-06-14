#include <iostream>
#include <vector>
#include "../include/models.h"
#include "../include/auth.h"

using namespace std;

void menuCRUDOlehKoder2() {
    cout << "\n[MENU CRUD AKTIF] - Bagian ini akan dikerjakan Koder 2...\n";
}

void menuKalkulatorOlehKoder3() {
    cout << "\n[MENU KALKULATOR AKTIF] - Bagian ini akan dikerjakan Koder 3...\n";
}

int main() {
    vector<User> databaseUser;
    
    // 1. LOAD DATA DARI FILE .TXT SAAT APLIKASI DIJALANKAN
    loadUsersDariFile(databaseUser);

    // LOOPING TERLUAR: Menjaga aplikasi tetap hidup sampai user memilih keluar aplikasi (0)
    while (true) {
        User userLogedIn;
        bool isSessionActive = false;
        string menuAwalStr; 

        cout << "======================================\n";
        cout << "       SELAMAT DATANG DI WARISKU       \n";
        cout << "======================================\n";

        // 2. GERBANG UTAMA (LOOPING SEBELUM LOGIN SUKSES)
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
                loadUsersDariFile(databaseUser); // Load ulang database agar akun baru langsung aktif
            } else if (menuAwalStr == "0") {
                cout << "Keluar dari aplikasi. Sampai jumpa!\n";
                return 0; // Menghentikan program dari layar awal
            } else {
                cout << " [!] Error: Pilihan tidak valid!\n";
            }
        } while (!isSessionActive);

        // 3. JIKA LOGIN SUKSES, MASUK MENU UTAMA BERDASARKAN ROLE
        cout << "\nLogin Sukses! Selamat Datang, " << userLogedIn.username << " (" << userLogedIn.role << ")\n";
        
        string pilihanMenuStr; 
        bool isLogout = false;

        do {
            cout << "\n============ MENU UTAMA ============\n";
            if (userLogedIn.role == "Notaris") {
                cout << "1. Kelola Data Aset & Keluarga\n";
                cout << "2. Hitung & Export Pembagian Waris\n";
            } else if (userLogedIn.role == "AhliWaris") {
                cout << "1. Lihat Pembagian Waris & Status Klaim\n";
            }
            // 👇 MENU LOGOUT & KELUAR APLIKASI DIPISAH 👇
            cout << "9. Logout (Kembali ke Menu Awal)\n";
            cout << "0. Keluar Aplikasi\n";
            cout << "Pilih Menu: "; 
            getline(cin >> ws, pilihanMenuStr);

            if (pilihanMenuStr == "1") {
                if (userLogedIn.role == "Notaris") {
                    menuCRUDOlehKoder2(); // RAHMAT UBAH INI BUAT NAMBAH FITUR
                } else {
                    cout << "\n[Fitur Ahli Waris] Menampilkan hasil porsi waris...\n"; // LULUT UBAH KODE INI HAPUS COUTNYA
                }
            } else if (pilihanMenuStr == "2" && userLogedIn.role == "Notaris") {
                menuKalkulatorOlehKoder3();
            } else if (pilihanMenuStr == "9") {
                cout << "\nLogout berhasil. Kembali ke Menu Awal...\n";
                isLogout = true; // Set flag true untuk memutus sesi dan kembali ke layar Login
            } else if (pilihanMenuStr == "0") {
                cout << "\nKeluar dari aplikasi. Sampai jumpa!\n";
                return 0; // Menghentikan program sepenuhnya dari dalam Menu Utama
            } else {
                cout << " [!] Error: Pilihan tidak valid!\n";
            }

        } while (!isLogout); // Keluar dari loop menu utama jika user memilih logout (9)
    }

    return 0;
}