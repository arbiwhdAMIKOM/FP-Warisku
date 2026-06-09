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

    User userLogedIn;
    bool isSessionActive = false;
    int menuAwal;

    cout << "======================================\n";
    cout << "       SELAMAT DATANG DI WARISKU       \n";
    cout << "======================================\n";

    // 2. GERBANG UTAMA (LOOPING SEBELUM LOGIN SUKSES)
    do {
        cout << "\n1. Login\n2. Register\n0. Keluar Aplikasi\nPilih opsi: ";
        cin >> menuAwal;

        if (menuAwal == 1) {
            isSessionActive = prosesLogin(databaseUser, userLogedIn);
            if (!isSessionActive) {
                cout << "Login Gagal! Username atau Password salah.\n";
            }
        } else if (menuAwal == 2) {
            prosesRegister(databaseUser);
        } else if (menuAwal == 0) {
            cout << "Keluar dari aplikasi. Sampai jumpa!\n";
            return 0;
        }
    } while (!isSessionActive);

    // 3. JIKA LOGIN SUKSES, MASUK MENU UTAMA BERDASARKAN ROLE
    cout << "\nLogin Sukses! Selamat Datang, " << userLogedIn.username << " (" << userLogedIn.role << ")\n";
    
    int pilihanMenu;
    do {
        cout << "\n============ MENU UTAMA ============\n";
        if (userLogedIn.role == "Notaris") {
            cout << "1. Kelola Data Aset & Keluarga\n";
            cout << "2. Hitung & Export Pembagian Waris\n";
        } else if (userLogedIn.role == "AhliWaris") {
            cout << "1. Lihat Pembagian Waris & Status Klaim\n";
        }
        cout << "0. Logout / Keluar\n";
        cout << "Pilih Menu: "; cin >> pilihanMenu;

        if (pilihanMenu == 1) {
            if (userLogedIn.role == "Notaris") {
                menuCRUDOlehKoder2(); // RAHMAT UBAH INI BUAT NAMBAH FITUR
            } else {
                cout << "\n[Fitur Ahli Waris] Menampilkan hasil porsi waris...\n"; // LULUT UBAH KODE INI HAPUS COUTNYA
            }
        } else if (pilihanMenu == 2 && userLogedIn.role == "Notaris") {
            menuKalkulatorOlehKoder3();
        }

    } while (pilihanMenu != 0);

    cout << "\nSesi Anda berakhir. Terima kasih telah menggunakan Warisku!\n";
    return 0;
}