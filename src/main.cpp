#include <iostream>
#include <vector>
#include <filesystem> 
#include "../include/models.h"
#include "../include/auth.h"
#include "../include/crud.h"
#include "../include/logic.h"
#include "../include/ui.h"

using namespace std;
namespace fs = std::filesystem; 

void jeda();
void clearScr();

// ==========================================
// MAIN & MENU UTAMA APLIKASI
// ==========================================
int main() {
    vector<User> databaseUser;
    loadUsersDariFile(databaseUser);

    while (true) {
        User userLogedIn;
        bool isSessionActive = false;

        do {
            int pilihanAwal = menuInteraktif("SELAMAT DATANG DI WARISKU", {"Login Akun", "Register Akun Baru", "Keluar Aplikasi"});
            if (pilihanAwal == 0) {
                isSessionActive = prosesLogin(databaseUser, userLogedIn);
                if (!isSessionActive) { cout << " [!] Login Gagal! Username/Password salah.\n"; cin.get(); }
            } else if (pilihanAwal == 1) {
                prosesRegister(databaseUser);
                loadUsersDariFile(databaseUser); 
                cin.get();
            } else if (pilihanAwal == 2) {
                system("clear"); cout << "\nKeluar dari aplikasi. Sampai jumpa!\n"; cin.get(); return 0; 
            }
        } while (!isSessionActive);

        system("clear");
        cout << "\n [v] Login Sukses! Selamat Datang, " << userLogedIn.username << " (" << userLogedIn.role << ")\n";
        cin.get();

        bool isLogout = false;
        string kasusAktif = "kasus_default.txt"; 

        do {
            string header = "MENU " + userLogedIn.role + " | Kasus: " + kasusAktif;
            vector<string> opsiMenu;

            if (userLogedIn.role == "Notaris") {
                opsiMenu = {
                    "Pilih Berkas Kasus Keluarga",
                    "Kelola Data Aset & Keluarga (CRUD)",
                    "Hitung Pembagian Harta Waris",
                    "Cetak Akta Resmi Pembagian Waris (HTML/PDF)",
                    "Verifikasi Klaim Ahli Waris",
                    "Logout", "Keluar Aplikasi"
                };
            } else if (userLogedIn.role == "AhliWaris") {
                opsiMenu = {
                    "Pilih Berkas Keluarga Anda",
                    "Lihat Pembagian Waris & Status Klaim",
                    "Unduh SKHW Keluarga (HTML/PDF)",
                    "Logout", "Keluar Aplikasi"
                };
            }

            int pil = menuInteraktif(header, opsiMenu);

            if (userLogedIn.role == "Notaris") {
                if (pil == 0) { 
                    // FITUR AUTO SCAN FOLDER (NOTARIS)
                    vector<string> listFile;
                    string pathFolder = "../database/";
                    if (fs::exists(pathFolder)) {
                        for (const auto& entry : fs::directory_iterator(pathFolder)) {
                            string namaFile = entry.path().filename().string();
                            if (namaFile.find(".txt") != string::npos && namaFile != "user.txt") {
                                listFile.push_back(namaFile);
                            }
                        }
                    }
                    listFile.push_back("[ + Buat Berkas Kasus Baru ]");
                    listFile.push_back("[ Batal ]");

                    int pilFile = menuInteraktif("DAFTAR BERKAS KASUS", listFile);
                    if (pilFile == (int)listFile.size() - 2) { 
                        system("clear");
                        cout << "Ketik Nama Kasus Baru (Tanpa spasi, misal 'Bahlil'): ";
                        string namaBaru;
                        cin >> namaBaru;
                        cin.ignore(10000, '\n');
                        kasusAktif = namaBaru + ".txt";
                    } else if (pilFile != (int)listFile.size() - 1) { 
                        kasusAktif = listFile[pilFile];
                    }
                }
                else if (pil == 1) menuCRUDRahmat(kasusAktif); 
                else if (pil == 2) menuKalkulatorWaris(kasusAktif);
                else if (pil == 3) eksporSuratNotaris(kasusAktif);
                else if (pil == 4) menuVerifikasiKlaimNotaris(kasusAktif);
                else if (pil == 5) isLogout = true;
                else if (pil == 6) { system("clear"); cout << "\nSampai jumpa, Notaris!\n"; cin.get(); return 0; }
            } 
            else if (userLogedIn.role == "AhliWaris") {
                if (pil == 0) { 
                    // FITUR AUTO SCAN FOLDER (AHLI WARIS)
                    vector<string> listFile;
                    string pathFolder = "../database/";
                    if (fs::exists(pathFolder)) {
                        for (const auto& entry : fs::directory_iterator(pathFolder)) {
                            string namaFile = entry.path().filename().string();
                            if (namaFile.find(".txt") != string::npos && namaFile != "user.txt" && namaFile != "kasus_default.txt") {
                                listFile.push_back(namaFile);
                            }
                        }
                    }
                    listFile.push_back("[ Batal ]");

                    int pilFile = menuInteraktif("PILIH BERKAS KELUARGA ANDA", listFile);
                    if (pilFile != (int)listFile.size() - 1) {
                        kasusAktif = listFile[pilFile];
                    }
                }
                else if (pil == 1) lihatInformasiPorsiWaris(kasusAktif); 
                else if (pil == 2) eksporSuratAhliWaris(kasusAktif);
                else if (pil == 3) isLogout = true;
                else if (pil == 4) { system("clear"); cout << "\nSampai jumpa, Ahli Waris!\n"; cin.get(); return 0; }
            }
        } while (!isLogout);
    }
    return 0;
}