#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
#include "../include/logic.h"

using namespace std;

// Inisialisasi variabel global
double totalHartaWarisan = 0;
double bagianIstri = 0;
double bagianAyah = 0;
double bagianIbu = 0;
int jumlahAnakLaki = 0; 
int jumlahAnakPerempuan = 0;
bool ayahAda = false;
bool ibuAda = false;
double porsiPerAnakLaki = 0;
double porsiPerAnakPerempuan = 0;

bool statusPerhitungan = false;
string statusKlaim = "BELUM DIKLAIM";
string nomorRekeningAhliWaris = "";

string formatRupiah(double angka) {
    string strAngka = to_string((long long)angka);
    string hasil = "";
    int count = 0;
    for (int i = strAngka.length() - 1; i >= 0; i--) {
        hasil += strAngka[i];
        count++;
        if (count % 3 == 0 && i != 0) {
            hasil += '.';
        }
    }
    reverse(hasil.begin(), hasil.end());
    return hasil;
}

// FUNGSI 1: Kalkulator Pembagian (Oleh Notaris)
void menuKalkulatorWaris() {
    cout << "\n========================================" << endl;
    cout << "       KALKULATOR PEMBAGIAN WARIS       " << endl;
    cout << "========================================" << endl;
    
    cout << "Masukkan Total Harta Warisan (Rp): "; cin >> totalHartaWarisan;
    cout << "Apakah Ayah masih hidup? (1=Ya, 0=Tidak): "; cin >> ayahAda;
    cout << "Apakah Ibu masih hidup? (1=Ya, 0=Tidak) : "; cin >> ibuAda;
    cout << "Masukkan Jumlah Anak Laki-Laki     : "; cin >> jumlahAnakLaki;
    cout << "Masukkan Jumlah Anak Perempuan     : "; cin >> jumlahAnakPerempuan;
    
    bool punyaAnak = (jumlahAnakLaki > 0 || jumlahAnakPerempuan > 0);
    double sisaHarta = totalHartaWarisan;

    bagianIstri = punyaAnak ? (totalHartaWarisan * 0.125) : (totalHartaWarisan * 0.25);
    sisaHarta -= bagianIstri;

    if (ibuAda) {
        bagianIbu = punyaAnak ? (totalHartaWarisan / 6.0) : (totalHartaWarisan / 3.0);
        sisaHarta -= bagianIbu;
    } else {
        bagianIbu = 0;
    }

    if (!punyaAnak) {
        porsiPerAnakLaki = 0;
        porsiPerAnakPerempuan = 0;
        if (ayahAda) {
            bagianAyah = sisaHarta;
            sisaHarta = 0;
        } else {
            bagianAyah = 0;
        }
    } else {
        if (ayahAda) {
            bagianAyah = totalHartaWarisan / 6.0;
            sisaHarta -= bagianAyah;
        } else {
            bagianAyah = 0;
        }

        int totalPoinRasio = (jumlahAnakLaki * 2) + (jumlahAnakPerempuan * 1);
        if (totalPoinRasio > 0) {
            double nilaiPerSatuPoin = sisaHarta / totalPoinRasio;
            porsiPerAnakLaki = nilaiPerSatuPoin * 2;
            porsiPerAnakPerempuan = nilaiPerSatuPoin * 1;
            sisaHarta = 0;
        }
    }
    
    statusPerhitungan = true;
    statusKlaim = "SIAP DIKLAIM";

    cout << "\n[Sukses] Perhitungan Berhasil Disimpan!" << endl;
    cout << "-> Bagian Istri               : Rp " << formatRupiah(bagianIstri) << endl;
    if (ibuAda)  cout << "-> Bagian Ibu                 : Rp " << formatRupiah(bagianIbu) << endl;
    if (ayahAda) cout << "-> Bagian Ayah                : Rp " << formatRupiah(bagianAyah) << endl;
    if (jumlahAnakLaki > 0) cout << "-> Bagian per Anak Laki       : Rp " << formatRupiah(porsiPerAnakLaki) << " /anak" << endl;
    if (jumlahAnakPerempuan > 0) cout << "-> Bagian per Anak Perempuan  : Rp " << formatRupiah(porsiPerAnakPerempuan) << " /anak" << endl;
    cout << "========================================" << endl;
    
    char pilEkspor;
    cout << "Apakah Anda ingin mengekspor hasil ini ke .csv? (y/n): "; cin >> pilEkspor;
    if (pilEkspor == 'y' || pilEkspor == 'Y') eksporKeCSV();
}

// Lihat Informasi & Ajukan Klaim (Oleh Ahli Waris)
void lihatInformasiPorsiWaris() {
    if (!statusPerhitungan) {
        cout << "\n[!] Notaris belum melakukan kalkulasi data harta waris.\n";
        return;
    }

    cout << "\n========================================" << endl;
    cout << "       LIHAT PORSI PEMBAGIAN WARIS      " << endl;
    cout << "========================================" << endl;
    cout << "Total Harta Warisan Keluarga : Rp " << formatRupiah(totalHartaWarisan) << endl;
    cout << "----------------------------------------" << endl;
    cout << "Rincian Hak Waris Terdaftar:\n";
    cout << "1. Porsi Istri            : Rp " << formatRupiah(bagianIstri) << endl;
    if (ibuAda)  cout << "2. Porsi Ibu              : Rp " << formatRupiah(bagianIbu) << endl;
    if (ayahAda) cout << "3. Porsi Ayah             : Rp " << formatRupiah(bagianAyah) << endl;
    if (jumlahAnakLaki > 0)    cout << "4. Porsi per Anak Laki    : Rp " << formatRupiah(porsiPerAnakLaki) << endl;
    if (jumlahAnakPerempuan > 0) cout << "5. Porsi per Anak Pr      : Rp " << formatRupiah(porsiPerAnakPerempuan) << endl;
    cout << "----------------------------------------" << endl;
    cout << "Status Klaim Dana: [" << statusKlaim << "]\n"; 
    cout << "========================================" << endl;

    if (statusKlaim == "SIAP DIKLAIM") {
        char pilihanKlaim;
        cout << "\nApakah Anda ingin mencairkan/klaim porsi dana ini sekarang? (y/n): ";
        cin >> pilihanKlaim;

        if (pilihanKlaim == 'y' || pilihanKlaim == 'Y') {
            cout << "-> Masukkan Nomor Rekening Bank Anda: ";
            cin >> nomorRekeningAhliWaris;

            statusKlaim = "MENUNGGU VERIFIKASI NOTARIS"; 
            cout << "\n[Sukses] Pengajuan klaim Anda telah dikirim ke pihak Notaris!\n";
        }
    }
}

// Menu Verifikasi Persetujuan Klaim (Oleh Notaris)
void menuVerifikasiKlaimNotaris() {
    cout << "\n========================================" << endl;
    cout << "       HALAMAN VERIFIKASI KLAIM        " << endl;
    cout << "========================================" << endl;

    if (statusKlaim == "BELUM DIKLAIM" || statusKlaim == "SIAP DIKLAIM") {
        cout << "[Data] Belum ada berkas berkas pengajuan pencairan dari Ahli Waris.\n";
    } 
    else if (statusKlaim == "MENUNGGU VERIFIKASI NOTARIS") {
        cout << "[Pemberitahuan] Ada 1 Klaim Pencairan Waris Masuk!\n";
        cout << "-> Transfer Ke Rekening Ahli Waris: " << nomorRekeningAhliWaris << endl;
        
        char pilihanVerifikasi;
        cout << "Setujui berkas hukum dan cairkan dana waris sekarang? (y/n): ";
        cin >> pilihanVerifikasi;

        if (pilihanVerifikasi == 'y' || pilihanVerifikasi == 'Y') {
            statusKlaim = "SELESAI / DICAIRKAN";
            cout << "\n[Sukses] Dana waris resmi dicairkan ke rekening ahli waris secara hukum.\n";
        } else {
            cout << "[Info] Verifikasi klaim ditangguhkan.\n";
        }
    } 
    else if (statusKlaim == "SELESAI / DICAIRKAN") {
        cout << "[Data] Semua klaim dana waris keluarga ini telah SELESAI diproses.\n";
    }
    cout << "========================================" << endl;
}

// Ekspor Laporan
void eksporKeCSV() {
    if (!statusPerhitungan) return;

    ofstream fileWaris("laporan_waris.csv");
    if (fileWaris.is_open()) {
        fileWaris << "Kategori,Jumlah/Status,Nilai Waris (Rp)\n";
        fileWaris << "Total Harta,-," << fixed << setprecision(0) << totalHartaWarisan << "\n";
        fileWaris << "Istri,-," << fixed << setprecision(0) << bagianIstri << "\n";
        if (ibuAda)  fileWaris << "Ibu,Ada," << fixed << setprecision(0) << bagianIbu << "\n";
        if (ayahAda) fileWaris << "Ayah,Ada," << fixed << setprecision(0) << bagianAyah << "\n";
        if (jumlahAnakLaki > 0) fileWaris << "Anak Laki-Laki," << jumlahAnakLaki << "," << fixed << setprecision(0) << porsiPerAnakLaki << "\n";
        if (jumlahAnakPerempuan > 0) fileWaris << "Anak Perempuan," << jumlahAnakPerempuan << "," << fixed << setprecision(0) << porsiPerAnakPerempuan << "\n";
        fileWaris << "Status Akhir,-," << statusKlaim << "\n"; 
        
        fileWaris.close();
        cout << "\n[Sukses] Berhasil mengekspor 'laporan_waris.csv'.\n";
    }
}