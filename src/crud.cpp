#include "../include/crud.h"
#include "../include/logic.h"
#include "../include/ui.h" // Engine UI
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

void loadSemuaData(const string& namaFile, string& namaPewaris, vector<Aset>& daftarAset, vector<AhliWaris>& daftarWaris) {
    ifstream file("../database/" + namaFile);
    if (!file.is_open()) {
        namaPewaris = "-"; statusKlaim = "BELUM DIKLAIM"; nomorRekeningAhliWaris = ""; statusPerhitungan = false; return;
    }
    string baris, mode = "";
    daftarAset.clear();
    daftarWaris.clear();

    while (getline(file, baris)) {
        if (baris.empty()) continue;
        if (baris == "[PEWARIS]" || baris == "[DAFTAR_ASET]" || baris == "[DAFTAR_KELUARGA]") {
            mode = baris; continue;
        }
        if (mode == "[PEWARIS]") namaPewaris = baris;
        else if (mode == "[DAFTAR_ASET]") {
            stringstream ss(baris);
            string namaAset, stringNilai;
            getline(ss, namaAset, ';'); getline(ss, stringNilai, ';');
            daftarAset.push_back({namaAset, stod(stringNilai)});
        } 
        else if (mode == "[DAFTAR_KELUARGA]") {
            stringstream ss(baris);
            string namaWaris, hub, strPorsi, strKlaim;
            getline(ss, namaWaris, ';'); getline(ss, hub, ';');
            getline(ss, strPorsi, ';'); getline(ss, strKlaim, ';');
            daftarWaris.push_back({namaWaris, hub, strPorsi.empty() ? 0 : stod(strPorsi), (strKlaim == "1")});
        }
    }
    file.close();
}

void simpanSemuaData(const string& namaFile, const string& namaPewaris, const vector<Aset>& daftarAset, const vector<AhliWaris>& daftarWaris) {
    ofstream file("../database/" + namaFile, ios::trunc); if (!file.is_open()) return;
    file << "[METADATA]\n" << statusKlaim << ";" << nomorRekeningAhliWaris << ";" << (statusPerhitungan ? "1" : "0") << "\n\n";
    file << "[PEWARIS]\n" << namaPewaris << "\n\n";
    file << "[DAFTAR_ASET]\n";
    for (const auto& aset : daftarAset) file << aset.nama << ";" << fixed << setprecision(0) << aset.nilaiRupiah << ";" << aset.kategori << ";" << aset.detailKategori << "\n";
    file << "\n[DAFTAR_KELUARGA]\n";
    for (const auto& waris : daftarWaris) file << waris.nama << ";" << waris.hubungan << ";" << fixed << setprecision(0) << waris.porsiUang << ";" << (waris.isKlaim ? "1" : "0") << ";" << waris.tanggalLahir << ";" << waris.pekerjaan << "\n";
    file.close();
}

void tampilkanRingkasanData(const string& namaPewaris, const vector<Aset>& daftarAset, const vector<AhliWaris>& daftarWaris) {
    cout << "\n=======================================================\n               RINGKASAN DATA PEWARISAN                 \n=======================================================\n";
    cout << "       PEWARIS       : " << namaPewaris << "\n       STATUS BERKAS : " << statusKlaim << "\n-------------------------------------------------------\n";
    double totalAset = 0; cout << " DAFTAR INVENTARIS ASET:\n";
    if (daftarAset.empty()) cout << "   (Belum ada data aset)\n";
    for (size_t i = 0; i < daftarAset.size(); ++i) {
        cout << "   " << (i+1) << ". " << left << setw(18) << daftarAset[i].nama 
             << " : Rp. " << formatRupiah(daftarAset[i].nilaiRupiah) << "\n";
        totalAset += daftarAset[i].nilaiRupiah;
    }
    cout << "   -------------------------------------------- +\n";
    cout << "   TOTAL NILAI ASET   : Rp. " << formatRupiah(totalAset) << "\n\n";

    cout << " DAFTAR AHLI WARIS & PORSI BAGIAN:\n";
    if (daftarWaris.empty()) cout << "   (Belum ada data keluarga)\n";
    for (size_t i = 0; i < daftarWaris.size(); ++i) {
        cout << "   " << (i+1) << ". " << left << setw(15) << daftarWaris[i].nama 
             << " (" << setw(10) << daftarWaris[i].hubungan << ") -> Porsi: Rp. " 
             << formatRupiah(daftarWaris[i].porsiUang) << "\n";
    }
    cout << "=======================================================\n";
    cout << "Tekan Enter untuk kembali..."; cin.get();
}

void menuCRUDRahmat(const string& namaFile) {
    string namaPewaris = "-";
    vector<Aset> daftarAset;
    vector<AhliWaris> daftarWaris;
    loadSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);

    while (true) {
        vector<string> opsiMenu = {
            "1. Input / Tambah Data Aset & Keluarga",
            "2. Tampilkan Ringkasan Berkas Kasus",
            "3. Edit / Ubah Komponen Kasus",
            "4. Hapus Komponen Kasus",
            "0. Kembali ke Menu Utama"
        };
        int pilihanIndex = menuInteraktif("MENU KELOLA DATA (" + namaFile + ")", opsiMenu);

        if (pilihanIndex == 0) {
            // INPUT DATA (TANPA NGETIK "SELESAI")
            system("clear");
            if (namaPewaris == "-" || namaPewaris.empty()) {
                cout << " Masukkan Nama Pewaris: "; getline(cin >> ws, namaPewaris);
            }
            // Tambah Aset
            while (true) {
                if (menuInteraktif("Tambah Aset Baru?", {"Ya", "Tidak (Selesai)"}) == 1) break;
                Aset asetBaru;
                system("clear"); cout << "\n--- TAMBAH ASET ---\n";
                cout << " Nama Aset : "; getline(cin >> ws, asetBaru.nama);
                asetBaru.nilaiRupiah = inputRupiahLive();
                daftarAset.push_back(asetBaru);
            }
            // Tambah Keluarga
            while (true) {
                if (menuInteraktif("Tambah Anggota Keluarga?", {"Ya", "Tidak (Selesai)"}) == 1) break;
                AhliWaris wBaru;
                system("clear"); cout << "\n--- TAMBAH KELUARGA ---\n";
                cout << " Nama Anggota: "; getline(cin >> ws, wBaru.nama);
                vector<string> opsiHub = {"Istri", "Ayah", "Ibu", "Anak Laki-laki", "Anak Perempuan"};
                int pilHub = menuInteraktif("Pilih Hubungan Keluarga", opsiHub);
                wBaru.hubungan = opsiHub[pilHub];
                wBaru.porsiUang = 0; wBaru.isKlaim = false;
                daftarWaris.push_back(wBaru);
            }
            simpanSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);
            
        } else if (pilihanIndex == 1) {
            system("clear"); tampilkanRingkasanData(namaPewaris, daftarAset, daftarWaris);
        } else if (pilihanIndex == 2) {
            // EDIT DATA INTERAKTIF
            int pilEdit = menuInteraktif("EDIT DATA KASUS", {"Ubah Nama Pewaris", "Ubah Aset", "Ubah Keluarga", "Batal"});
            if (pilEdit == 0) {
                system("clear"); cout << "Nama Baru: "; getline(cin >> ws, namaPewaris);
            } else if (pilEdit == 1 && !daftarAset.empty()) {
                vector<string> opt; for(auto& a: daftarAset) opt.push_back(a.nama + " (Rp." + formatRupiah(a.nilaiRupiah) + ")");
                opt.push_back("Batal");
                int no = menuInteraktif("Pilih Aset:", opt);
                if(no != opt.size()-1) {
                    system("clear"); cout << "Nama Baru: "; getline(cin >> ws, daftarAset[no].nama);
                    daftarAset[no].nilaiRupiah = inputRupiahLive();
                }
            } else if (pilEdit == 2 && !daftarWaris.empty()) {
                vector<string> opt; for(auto& k: daftarWaris) opt.push_back(k.nama + " (" + k.hubungan + ")");
                opt.push_back("Batal");
                int no = menuInteraktif("Pilih Keluarga:", opt);
                if(no != opt.size()-1) {
                    system("clear"); cout << "Nama Baru: "; getline(cin >> ws, daftarWaris[no].nama);
                    vector<string> oHub = {"Istri", "Ayah", "Ibu", "Anak Laki-laki", "Anak Perempuan"};
                    daftarWaris[no].hubungan = oHub[menuInteraktif("Pilih Hubungan:", oHub)];
                }
            }
            simpanSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);

        } else if (pilihanIndex == 3) {
            // HAPUS DATA INTERAKTIF
            int pilHapus = menuInteraktif("HAPUS DATA KASUS", {"Hapus Aset", "Hapus Keluarga", "Batal"});
            if (pilHapus == 0 && !daftarAset.empty()) {
                vector<string> opt; for(auto& a: daftarAset) opt.push_back(a.nama); opt.push_back("Batal");
                int no = menuInteraktif("Hapus Aset:", opt);
                if(no != opt.size()-1) daftarAset.erase(daftarAset.begin() + no);
            } else if (pilHapus == 1 && !daftarWaris.empty()) {
                vector<string> opt; for(auto& k: daftarWaris) opt.push_back(k.nama); opt.push_back("Batal");
                int no = menuInteraktif("Hapus Keluarga:", opt);
                if(no != opt.size()-1) daftarWaris.erase(daftarWaris.begin() + no);
            }
            simpanSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);
        } else if (pilihanIndex == 4) {
            break;
        }
    }
}