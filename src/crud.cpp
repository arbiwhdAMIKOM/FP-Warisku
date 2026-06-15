#include "../include/crud.h"
#include "../include/logic.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

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

void loadSemuaData(const string& namaFile, string& namaPewaris, vector<Aset>& daftarAset, vector<AhliWaris>& daftarWaris) {
    ifstream file("../database/" + namaFile);
    if (!file.is_open()) {
        namaPewaris = "-";
        return;
    }

    string baris;
    string mode = "";
    daftarAset.clear();
    daftarWaris.clear();

    while (getline(file, baris)) {
        if (baris.empty()) continue;
        
        if (baris == "[PEWARIS]" || baris == "[DAFTAR_ASET]" || baris == "[DAFTAR_KELUARGA]") {
            mode = baris;
            continue;
        }

        if (mode == "[PEWARIS]") {
            namaPewaris = baris;
        } 
        else if (mode == "[DAFTAR_ASET]") {
            stringstream ss(baris);
            string namaAset, stringNilai;
            getline(ss, namaAset, ';');
            getline(ss, stringNilai, ';');
            daftarAset.push_back({namaAset, stod(stringNilai)});
        } 
        else if (mode == "[DAFTAR_KELUARGA]") {
            stringstream ss(baris);
            string namaWaris, hubungan, stringPorsi, stringKlaim;
            getline(ss, namaWaris, ';');
            getline(ss, hubungan, ';');
            getline(ss, stringPorsi, ';');
            getline(ss, stringKlaim, ';');

            double porsi = stringPorsi.empty() ? 0 : stod(stringPorsi);
            bool klaim = (stringKlaim == "1");

            daftarWaris.push_back({namaWaris, hubungan, porsi, klaim});
        }
    }
    file.close();
}


void simpanSemuaData(const string& namaFile, const string& namaPewaris, const vector<Aset>& daftarAset, const vector<AhliWaris>& daftarWaris) {
    ofstream file("../database/" + namaFile, ios::trunc);
    if (!file.is_open()) return;

    file << "[PEWARIS]\n" << namaPewaris << "\n\n";

    file << "[DAFTAR_ASET]\n";
    for (const auto& aset : daftarAset) {
        file << aset.nama << ";" << fixed << setprecision(0) << aset.nilaiRupiah << "\n";
    }
    file << "\n";

    file << "[DAFTAR_KELUARGA]\n";
    for (const auto& waris : daftarWaris) {
        file << waris.nama << ";" 
             << waris.hubungan << ";" 
             << fixed << setprecision(0) << waris.porsiUang << ";" 
             << (waris.isKlaim ? "1" : "0") << "\n";
    }
    
    file.close();
}


void prosesInputSistemWarisan(const string& namaFile, string& namaPewaris, vector<Aset>& daftarAset, vector<AhliWaris>& daftarWaris) {
    cout << "\n==================================================\n";
    cout << "          PENCATATAN DATA WARIS (" << namaFile << ")\n";
    cout << "==================================================\n";
    
    if (namaPewaris == "-" || namaPewaris.empty()) {
        cout << " [1] Masukkan Nama Pewaris: ";
        getline(cin >> ws, namaPewaris);
    } else {
        cout << " [1] Nama Pewaris: " << namaPewaris << " (Sudah Tercatat)\n";
    }
    
    cout << "\n [2] Masukkan Daftar Aset (Ketik 'selesai' untuk menyudahi)\n";
    while (true) {
        Aset asetBaru;
        cout << "     - Nama Aset : ";
        getline(cin >> ws, asetBaru.nama);
        if (asetBaru.nama == "selesai" || asetBaru.nama == "Selesai") break;
        cout << "       Nilai (Rp): ";
        cin >> asetBaru.nilaiRupiah;
        daftarAset.push_back(asetBaru);
    }

    cout << "\n [3] Masukkan Daftar Anggota Keluarga (Ketik 'selesai' untuk menyudahi)\n";
    while (true) {
        AhliWaris warisBaru;
        cout << "     - Nama Anggota Keluarga: ";
        getline(cin >> ws, warisBaru.nama);
        if (warisBaru.nama == "selesai" || warisBaru.nama == "Selesai") break;
        cout << "       Hubungan Keluarga    : ";
        getline(cin >> ws, warisBaru.hubungan);
        warisBaru.porsiUang = 0; 
        warisBaru.isKlaim = false;
        daftarWaris.push_back(warisBaru);
    }

    simpanSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);
    cout << "\n [v] Data Berhasil Ditambahkan ke Kasus ini!\n";
}

void tampilkanRingkasanData(const string& namaPewaris, const vector<Aset>& daftarAset, const vector<AhliWaris>& daftarWaris) {
    cout << "\n=======================================================\n";
    cout << "              RINGKASAN DATA PEWARISAN                 \n";
    cout << "=======================================================\n";
    cout << "       PEWARIS       : " << namaPewaris << "\n";
    cout << "-------------------------------------------------------\n";
    
    double totalAset = 0;
    cout << " DAFTAR ASET:\n";
    if (daftarAset.empty()) cout << "   (Belum ada data aset)\n";
    for (size_t i = 0; i < daftarAset.size(); ++i) {
        cout << "   " << (i+1) << ". " << left << setw(18) << daftarAset[i].nama 
             << " : " << formatRupiah(daftarAset[i].nilaiRupiah) << "\n";
        totalAset += daftarAset[i].nilaiRupiah;
    }
    cout << "   -------------------------------------------- +\n";
    cout << "   TOTAL NILAI ASET   : " << formatRupiah(totalAset) << "\n\n";

    cout << " DAFTAR AHLI WARIS & PORSI BAGIAN:\n";
    if (daftarWaris.empty()) cout << "   (Belum ada data keluarga)\n";
    for (size_t i = 0; i < daftarWaris.size(); ++i) {
        cout << "   " << (i+1) << ". " << left << setw(15) << daftarWaris[i].nama 
             << " (" << setw(10) << daftarWaris[i].hubungan << ") -> Porsi: " 
             << formatRupiah(daftarWaris[i].porsiUang) 
             << (daftarWaris[i].porsiUang > 0 ? (daftarWaris[i].isKlaim ? " [Sudah Diklaim]" : " [Belum Diklaim]") : "") << "\n";
    }
    cout << "=======================================================\n";
}

void ubahData(const string& namaFile, string& namaPewaris, vector<Aset>& daftarAset, vector<AhliWaris>& daftarWaris) {
    int pilihan;
    clearScr();
    cout << "\n--- EDIT DATA KASUS ---\n";
    cout << " 1. Ubah Nama Pewaris\n 2. Ubah Salah Satu Aset\n 3. Ubah Salah Satu Keluarga\n Pilihan: ";
    cin >> pilihan;
    jeda();

    if (pilihan == 1) {
        cout << " Masukkan Nama Pewaris Baru: ";
        getline(cin >> ws, namaPewaris);
    } else if (pilihan == 2 && !daftarAset.empty()) {
        int no; cout << " Pilih nomor aset: "; cin >> no;
        if (no > 0 && no <= static_cast<int>(daftarAset.size())) {
            cout << " Nama baru: "; getline(cin >> ws, daftarAset[no-1].nama);
            cout << " Nilai baru: "; cin >> daftarAset[no-1].nilaiRupiah;
        }
    } else if (pilihan == 3 && !daftarWaris.empty()) {
        int no; cout << " Pilih nomor keluarga: "; cin >> no;
        if (no > 0 && no <= static_cast<int>(daftarWaris.size())) {
            cout << " Nama baru: "; getline(cin >> ws, daftarWaris[no-1].nama);
            cout << " Hubungan baru: "; getline(cin >> ws, daftarWaris[no-1].hubungan);
        }
    }
    simpanSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);
    cout << " [v] Data berhasil diperbarui!\n";
}

void hapusData(const string& namaFile, string& namaPewaris, vector<Aset>& daftarAset, vector<AhliWaris>& daftarWaris) {
    int pilihan;
    clearScr(); 
    cout << "\n--- HAPUS DATA KASUS ---\n";
    cout << " 1. Hapus Salah Satu Aset\n 2. Hapus Salah Satu Keluarga\n Pilihan: ";
    cin >> pilihan;
    jeda();

    if (pilihan == 1 && !daftarAset.empty()) {
        int no; cout << " Pilih nomor aset: "; cin >> no;
        if (no > 0 && no <= static_cast<int>(daftarAset.size())) daftarAset.erase(daftarAset.begin() + (no-1));
    } else if (pilihan == 2 && !daftarWaris.empty()) {
        int no; cout << " Pilih nomor keluarga: "; cin >> no;
        if (no > 0 && no <= static_cast<int>(daftarWaris.size())) daftarWaris.erase(daftarWaris.begin() + (no-1));
    }
    simpanSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);
    cout << " [v] Data berhasil dihapus!\n";
}