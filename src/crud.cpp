#include "../include/crud.h"
#include "../include/logic.h" 
#include "../include/ui.h" 
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>    
#include <algorithm>

using namespace std;

bool cekTahunKabisat(int tahun) {
    return (tahun % 4 == 0 && tahun % 100 != 0) || (tahun % 400 == 0);
}

bool validasiDanFormatTanggal(string input, string& tanggalHasil) {
    if (input.length() == 8 && all_of(input.begin(), input.end(), ::isdigit)) {
        input = input.substr(0, 2) + "-" + input.substr(2, 2) + "-" + input.substr(4, 4);
    }

    regex pola("^(\\d{2})-(\\d{2})-(\\d{4})$");
    smatch hasilCocok;
    if (!regex_match(input, hasilCocok, pola)) return false;

    int hari = stoi(hasilCocok[1].str());
    int bulan = stoi(hasilCocok[2].str());
    int tahun = stoi(hasilCocok[3].str());

    if (tahun < 1800 || tahun > 2026) return false; 
    if (bulan < 1 || bulan > 12) return false;

    int batasHariBulan[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (bulan == 2 && cekTahunKabisat(tahun)) batasHariBulan[2] = 29;

    if (hari < 1 || hari > batasHariBulan[bulan]) return false;

    tanggalHasil = input;
    return true;
}

void loadSemuaData(const string& namaFile, string& namaPewaris, vector<Aset>& daftarAset, vector<AhliWaris>& daftarWaris) {
    ifstream file("../database/" + namaFile);
    if (!file.is_open()) {
        namaPewaris = "-"; statusKlaim = "BELUM DIKLAIM"; nomorRekeningAhliWaris = ""; statusPerhitungan = false; return;
    }
    string baris, mode = "";
    daftarAset.clear(); daftarWaris.clear();
    while (getline(file, baris)) {
        if (baris.empty()) continue;
        if (baris == "[METADATA]" || baris == "[PEWARIS]" || baris == "[DAFTAR_ASET]" || baris == "[DAFTAR_KELUARGA]") {
            mode = baris; continue;
        }
        if (mode == "[METADATA]") {
            stringstream ss(baris); string strHitung;
            getline(ss, statusKlaim, ';'); getline(ss, nomorRekeningAhliWaris, ';'); getline(ss, strHitung, ';');
            statusPerhitungan = (strHitung == "1");
        }
        else if (mode == "[PEWARIS]") namaPewaris = baris;
        else if (mode == "[DAFTAR_ASET]") {
            stringstream ss(baris); string namaAset, stringNilai, kat, det;
            getline(ss, namaAset, ';'); getline(ss, stringNilai, ';'); getline(ss, kat, ';'); getline(ss, det, ';');
            daftarAset.push_back({namaAset, stod(stringNilai), kat, det});
        } 
        else if (mode == "[DAFTAR_KELUARGA]") {
            stringstream ss(baris); string namaWaris, hub, strPorsi, strKlaim, tglLahir, pek;
            getline(ss, namaWaris, ';'); getline(ss, hub, ';'); getline(ss, strPorsi, ';'); getline(ss, strKlaim, ';'); getline(ss, tglLahir, ';'); getline(ss, pek, ';');
            daftarWaris.push_back({namaWaris, hub, strPorsi.empty() ? 0 : stod(strPorsi), (strKlaim == "1"), tglLahir, pek});
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
        cout << "   " << (i+1) << ". [" << daftarAset[i].kategori << "] " << left << setw(15) << daftarAset[i].nama << "\n      Detail Spasial : " << setw(30) << daftarAset[i].detailKategori << "\n      Estimasi Nilai : Rp. " << formatRupiah(daftarAset[i].nilaiRupiah) << "\n\n";
        totalAset += daftarAset[i].nilaiRupiah;
    }
    cout << "   -------------------------------------------- +\n   TOTAL NILAI KEKAYAAN : Rp. " << formatRupiah(totalAset) << "\n\n DAFTAR AHLI WARIS SAH:\n";
    if (daftarWaris.empty()) cout << "   (Belum ada data keluarga)\n";
    for (size_t i = 0; i < daftarWaris.size(); ++i) cout << "   " << (i+1) << ". " << left << setw(12) << daftarWaris[i].nama << " | TTL: " << setw(11) << daftarWaris[i].tanggalLahir << " | Kerja: " << setw(10) << daftarWaris[i].pekerjaan << " | Peran: " << setw(14) << daftarWaris[i].hubungan << " -> Porsi: Rp. " << formatRupiah(daftarWaris[i].porsiUang) << "\n";
    cout << "=======================================================\n";
    cout << "Tekan Enter untuk kembali..."; cin.get();
}

void menuCRUDRahmat(const string& namaFile) {
    string namaPewaris = "-"; vector<Aset> daftarAset; vector<AhliWaris> daftarWaris;
    loadSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);

    while (true) {
        vector<string> opsiMenu = {"1. Input / Tambah Data Aset & Keluarga", "2. Tampilkan Ringkasan Berkas Kasus", "3. Edit / Ubah Komponen Kasus", "4. Hapus Komponen Kasus", "0. Kembali ke Menu Utama"};
        int pilihanIndex = menuInteraktif("MENU KELOLA DATA (" + namaFile + ")", opsiMenu);

        if (pilihanIndex == 0) {
            system("clear"); if (namaPewaris == "-" || namaPewaris.empty()) { cout << " Masukkan Nama Pewaris: "; getline(cin >> ws, namaPewaris); }
            while (true) {
                if (menuInteraktif("Tambah Aset Baru?", {"Ya", "Tidak (Selesai)"}) == 1) break;
                Aset asetBaru; vector<string> opsiKat = {"Kendaraan", "Rumah / Tanah", "Lainnya"};
                int pilKat = menuInteraktif("Pilih Kategori Komponen Aset", opsiKat); asetBaru.kategori = opsiKat[pilKat];
                system("clear"); cout << "\n--- TAMBAH ASET (" << asetBaru.kategori << ") ---\n Nama / Deskripsi Aset   : "; getline(cin >> ws, asetBaru.nama);
                if (pilKat == 0) { string brand; cout << " Detail Brand / Merk     : "; getline(cin >> ws, brand); asetBaru.detailKategori = "Brand: " + brand; } 
                else if (pilKat == 1) { string lokasi, luas; cout << " Lokasi Spasial Objek    : "; getline(cin >> ws, lokasi); cout << " Luas Wilayah Objek (m2) : "; getline(cin >> ws, luas); asetBaru.detailKategori = "Lokasi: " + lokasi + ", Luas: " + luas + " m2"; } 
                else { string ket; cout << " Keterangan Detail       : "; getline(cin >> ws, ket); asetBaru.detailKategori = ket; }
                asetBaru.nilaiRupiah = inputRupiahLive(); daftarAset.push_back(asetBaru);
            }
            while (true) {
                if (menuInteraktif("Tambah Anggota Keluarga?", {"Ya", "Tidak (Selesai)"}) == 1) break;
                AhliWaris wBaru; system("clear"); cout << "\n--- TAMBAH KELUARGA ---\n Nama Anggota Keluarga   : "; getline(cin >> ws, wBaru.nama);
                
                // INJEKSI LIVE TANGGAL INPUT BARU
                while (true) {
                    string rawTanggal = inputTanggalLive();
                    if (validasiDanFormatTanggal(rawTanggal, wBaru.tanggalLahir)) break;
                    cout << " [!] Error: Tanggal lahir tidak valid secara kalender! Coba lagi.\n";
                }

                cout << " Pekerjaan               : "; getline(cin >> ws, wBaru.pekerjaan);    
                vector<string> opsiHub = {"Istri", "Ayah", "Ibu", "Anak Laki-laki", "Anak Perempuan"};
                int pilHub = menuInteraktif("Pilih Peran Hubungan Keluarga", opsiHub); wBaru.hubungan = opsiHub[pilHub];
                wBaru.porsiUang = 0; wBaru.isKlaim = false; daftarWaris.push_back(wBaru);
            }
            simpanSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);
        } else if (pilihanIndex == 1) {
            system("clear"); tampilkanRingkasanData(namaPewaris, daftarAset, daftarWaris); cout << "Tekan Enter untuk kembali..."; cin.get();
        } else if (pilihanIndex == 2) {
            int pilEdit = menuInteraktif("EDIT DATA KASUS", {"Ubah Nama Pewaris", "Ubah Aset", "Ubah Keluarga", "Batal"});
            if (pilEdit == 0) { system("clear"); cout << "Nama Baru: "; getline(cin >> ws, namaPewaris); } 
            else if (pilEdit == 1 && !daftarAset.empty()) {
                vector<string> opt; for(auto& a: daftarAset) opt.push_back(a.nama + " [" + a.kategori + "]"); opt.push_back("Batal");
                int no = menuInteraktif("Pilih Aset Yang Ingin Diubah:", opt);
                if(no != (int)opt.size()-1) {
                    system("clear"); cout << "Nama Aset Baru: "; getline(cin >> ws, daftarAset[no].nama);
                    if (daftarAset[no].kategori == "Kendaraan") { string b; cout << "Brand Baru: "; getline(cin >> ws, b); daftarAset[no].detailKategori = "Brand: " + b; } 
                    else if (daftarAset[no].kategori == "Rumah / Tanah") { string lok, lu; cout << "Lokasi Baru: "; getline(cin >> ws, lok); cout << "Luas Baru (m2): "; getline(cin >> ws, lu); daftarAset[no].detailKategori = "Lokasi: " + lok + ", Luas: " + lu + " m2"; } 
                    else { string k; cout << "Keterangan Baru: "; getline(cin >> ws, k); daftarAset[no].detailKategori = k; }
                    daftarAset[no].nilaiRupiah = inputRupiahLive();
                }
            } else if (pilEdit == 2 && !daftarWaris.empty()) {
                vector<string> opt; for(auto& k: daftarWaris) opt.push_back(k.nama + " (" + k.hubungan + ")"); opt.push_back("Batal");
                int no = menuInteraktif("Pilih Anggota Keluarga Yang Ingin Diubah:", opt);
                if(no != (int)opt.size()-1) {
                    system("clear"); cout << "Nama Baru          : "; getline(cin >> ws, daftarWaris[no].nama);
                    
                    // INJEKSI LIVE TANGGAL MENU EDIT
                    while (true) {
                        string rawTanggal = inputTanggalLive(); 
                        if (validasiDanFormatTanggal(rawTanggal, daftarWaris[no].tanggalLahir)) break;
                        cout << " [!] Error: Tanggal lahir tidak valid secara kalender! Coba lagi.\n";
                    }

                    cout << "Pekerjaan Baru     : "; getline(cin >> ws, daftarWaris[no].pekerjaan);
                    vector<string> oHub = {"Istri", "Ayah", "Ibu", "Anak Laki-laki", "Anak Perempuan"};
                    daftarWaris[no].hubungan = oHub[menuInteraktif("Pilih Peran Hubungan Baru:", oHub)];
                }
            }
            simpanSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);
        } else if (pilihanIndex == 3) {
            int pilHapus = menuInteraktif("HAPUS DATA KASUS", {"Hapus Aset", "Hapus Keluarga", "Batal"});
            if (pilHapus == 0 && !daftarAset.empty()) { vector<string> opt; for(auto& a: daftarAset) opt.push_back(a.nama); opt.push_back("Batal"); int no = menuInteraktif("Hapus Aset:", opt); if(no != (int)opt.size()-1) daftarAset.erase(daftarAset.begin() + no); } 
            else if (pilHapus == 1 && !daftarWaris.empty()) { vector<string> opt; for(auto& k: daftarWaris) opt.push_back(k.nama); opt.push_back("Batal"); int no = menuInteraktif("Hapus Keluarga:", opt); if(no != (int)opt.size()-1) daftarWaris.erase(daftarWaris.begin() + no); }
            simpanSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);
        } else if (pilihanIndex == 4) break;
    }
}