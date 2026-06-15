#include <iostream>
#include <vector>
#include "../include/models.h"
#include "../include/auth.h"
#include "../include/crud.h"
#include "../include/logic.h"

using namespace std;

void jeda();
void clearScr();

void menuCRUDRahmat() {
    string namaFile = "data_waris.txt";
    string namaPewaris = "-";
    vector<Aset> daftarAset;
    vector<AhliWaris> daftarWaris;

    loadSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);

    string pilihanCRUD;
    while (true) {
        clearScr();
        cout << "\n======================================\n";
        cout << "     MENU KELOLA DATA   \n";
        cout << "======================================\n";
        cout << "1. Input / Tambah Data Kasus Baru\n";
        cout << "2. Tampilkan Ringkasan Berkas Kasus\n";
        cout << "3. Edit / Ubah Komponen Data Kasus\n";
        cout << "4. Hapus Komponen Data Kasus\n";
        cout << "0. Kembali ke Menu Utama\n";
        cout << "Pilih Opsi: ";
        getline(cin >> ws, pilihanCRUD);

        if (pilihanCRUD == "1") {
            clearScr();
            prosesInputSistemWarisan(namaFile, namaPewaris, daftarAset, daftarWaris);
            jeda();
        } else if (pilihanCRUD == "2") {
            clearScr();
            tampilkanRingkasanData(namaPewaris, daftarAset, daftarWaris);
            jeda();
        } else if (pilihanCRUD == "3") {
            clearScr();
            ubahData(namaFile, namaPewaris, daftarAset, daftarWaris);
            jeda();
        } else if (pilihanCRUD == "4") {
            clearScr();
            hapusData(namaFile, namaPewaris, daftarAset, daftarWaris);
            jeda();
        } else if (pilihanCRUD == "0") {
            break;
        } else {
            cout << " [!] Error: Pilihan tidak valid!\n";
            jeda();
        }
    }
}

void menuKalkulatorLulut() {
}

int main() {
    vector<User> databaseUser;
    
    loadUsersDariFile(databaseUser);

    while (true) {
        User userLogedIn;
        bool isSessionActive = false;
        string menuAwalStr; 

        clearScr();
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

        clearScr();
        cout << "\nLogin Sukses! Selamat Datang, " << userLogedIn.username << " (" << userLogedIn.role << ")\n";
        jeda();

        string pilihanMenuStr; 
        bool isLogout = false;

        do {
            clearScr();
            cout << "\n Login Sebagai: " << userLogedIn.username << " (" << userLogedIn.role << ")\n";
            cout << "\n============ MENU UTAMA ============\n";
            if (userLogedIn.role == "Notaris") {
                cout << "1. Kelola Data Aset & Keluarga\n";
                cout << "2. Hitung & Export Pembagian Waris\n";
                cout << "3. Verifikasi Klaim Ahli Waris\n"; // <-- Tambahan opsi menu verifikasi klaim jika diperlukan
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
                    lihatInformasiPorsiWaris(); 
                }
            } else if (pilihanMenuStr == "2" && userLogedIn.role == "Notaris") {
                menuKalkulatorWaris();
            } else if (pilihanMenuStr == "3" && userLogedIn.role == "Notaris") { // <-- Tambahan eksekusi menu verifikasi klaim jika diperlukan
                menuVerifikasiKlaimNotaris();
            } else if (pilihanMenuStr == "9") {
                clearScr();
                cout << "\nLogout berhasil. Kembali ke Menu Awal...\n";
                isLogout = true; 
                jeda();
            } else if (pilihanMenuStr == "0") {
                clearScr();
                cout << "\nKeluar dari aplikasi. Sampai jumpa!\n";
                jeda();
                return 0; 
            } else {
                clearScr();
                cout << " [!] Error: Pilihan tidak valid!\n";
                jeda();
            }

        } while (!isLogout); 
    }

    return 0;
}