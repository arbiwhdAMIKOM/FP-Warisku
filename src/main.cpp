#include <iostream>
#include <vector>
#include "../include/models.h"
#include "../include/auth.h"
<<<<<<< HEAD
#include "../include/crud.h"
=======
#include "../include/logic.h"
>>>>>>> a03c646eb54482954a7147f12abc7ff832a0bb36

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
    string namaFile, namaPewaris;
    vector<Aset> daftarAset;
    vector<AhliWaris> daftarKeluarga;

    cout << "==========================================\n";
    cout << "         SISTEM NOTARIS WARIS             \n";
    cout << "==========================================\n";
    cout << " Masukkan Nama File Kasus Pengguna \n";
    cout << " (Contoh: Budi_Santoso atau Kasus_A): ";
    cin >> namaFile;
    namaFile += ".txt"; // Otomatis sistem menambahkan ekstensi .txt

    // 1. Ambil data spesifik dari file kasus tersebut
    loadSemuaData(namaFile, namaPewaris, daftarAset, daftarKeluarga);

    int menu;
    do {
        cout << "\n--- MENU KASUS: " << (namaPewaris == "-" ? "KASUS BARU" : namaPewaris) << " ---\n";
        cout << " 1. Tambah Data Baru (Aset / Keluarga)\n";
        cout << " 2. Tampilkan Ringkasan Data & Porsi Waris\n";
        cout << " 3. Edit Data\n";
        cout << " 4. Hapus Data\n";
        cout << " 5. Ganti Kasus / Keluar\n";
        cout << " Pilih Menu: ";
        cin >> menu;

        switch(menu) {
            case 1:
                clearScreen();
                prosesInputSistemWarisan(namaFile, namaPewaris, daftarAset, daftarKeluarga);
                jedaLayar();
                break;
            case 2:
                clearScreen();
                tampilkanRingkasanData(namaPewaris, daftarAset, daftarKeluarga);
                jedaLayar();
                break;
            case 3:
                clearScreen();
                ubahData(namaFile, namaPewaris, daftarAset, daftarKeluarga);
                jedaLayar();
                break;
            case 4:
                clearScreen();
                hapusData(namaFile, namaPewaris, daftarAset, daftarKeluarga);
                jedaLayar();
                break;
            case 5:
                clearScreen();
                cout << " Sesi Kasus Ditutup.\n";
                jedaLayar();
                break;
            default:
                clearScreen();
                cout << " [!] Menu salah.\n";
                jedaLayar();
        }
    } while (menu != 5);;
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
                    jedaLayar();
                }
            } else if (menuAwalStr == "2") {
                prosesRegister(databaseUser);
                loadUsersDariFile(databaseUser); 
                jedaLayar();
            } else if (menuAwalStr == "0") {
                cout << "Keluar dari aplikasi. Sampai jumpa!\n";
                return 0; 
            } else {
                cout << " [!] Error: Pilihan tidak valid!\n";
                jedaLayar();
            }
        } while (!isSessionActive);

            clearScreen();
            cout << "Login Sukses! Selamat Datang, " << userLogedIn.username << " (" << userLogedIn.role << ")\n";
            jedaLayar();

        string pilihanMenuStr; 
        bool isLogout = false;

        do {
            clearScreen();
            cout << "\nLogin Sebagai: " << userLogedIn.username << " (" << userLogedIn.role << ")\n";
            cout << "\n============ MENU UTAMA ============\n";
            if (userLogedIn.role == "Notaris") {
                cout << "1. Kelola Data Aset & Keluarga\n";
                cout << "2. Hitung & Export Pembagian Waris\n";
                cout << "3. Verifikasi Klaim Ahli Waris\n";
            } else if (userLogedIn.role == "AhliWaris") {
                cout << "1. Lihat Pembagian Waris & Status Klaim\n";
            }
            cout << "9. Logout (Kembali ke Menu Awal)\n";
            cout << "0. Keluar Aplikasi\n";
            cout << "Pilih Menu: "; 
            getline(cin >> ws, pilihanMenuStr);

            if (pilihanMenuStr == "1") {
                if (userLogedIn.role == "Notaris") {
                    menuCRUDRahmat(); // Rahmat: Ganti baris ini dengan fungsi CRUD
                    jedaLayar();
                } else {
                    lihatInformasiPorsiWaris();
                    jedaLayar();
                }
            } else if (pilihanMenuStr == "2" && userLogedIn.role == "Notaris") {
                menuKalkulatorWaris();
                jedaLayar();
            } else if (pilihanMenuStr == "3" && userLogedIn.role == "Notaris") {
                menuVerifikasiKlaimNotaris();
                jedaLayar();
            } else if (pilihanMenuStr == "9") {
                cout << "\nLogout berhasil. Kembali ke Menu Awal...\n";
                isLogout = true; 
                jedaLayar();
            } else if (pilihanMenuStr == "0") {
                cout << "\nKeluar dari aplikasi. Sampai jumpa!\n";
                return 0; 
            } else {
                cout << " [!] Error: Pilihan tidak valid!\n";
                jedaLayar();
            }

        } while (!isLogout); 
    }

    return 0;
}