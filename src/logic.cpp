#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include "../include/logic.h"
#include "../include/models.h"
#include "../include/crud.h"
#include "../include/ui.h"

using namespace std;

double totalHartaWarisan = 0;
double bagianIstri = 0, bagianAyah = 0, bagianIbu = 0;
int jumlahAnakLaki = 0, jumlahAnakPerempuan = 0;
bool ayahAda = false, ibuAda = false;
double porsiPerAnakLaki = 0, porsiPerAnakPerempuan = 0;
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
        if (count % 3 == 0 && i != 0) hasil += '.';
    }
    reverse(hasil.begin(), hasil.end());
    return hasil;
}

void menuKalkulatorWaris(const string& namaFile) {
    string namaPewaris;
    vector<Aset> daftarAset;
    vector<AhliWaris> daftarWaris;

    system("clear");
    cout << "\n========================================" << endl;
    cout << "       KALKULATOR PEMBAGIAN WARIS       " << endl;
    cout << "========================================" << endl;

    loadSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);

    if(daftarAset.empty() || daftarWaris.empty()) {
        cout << " [!] Gagal: Data Kosong! Silakan isi dulu di Menu Kelola Data.\n";
        cout << "Tekan Enter..."; cin.get(); return;
    }

    totalHartaWarisan = 0;
    for(const auto& aset : daftarAset) totalHartaWarisan += aset.nilaiRupiah;

    ayahAda = ibuAda = false;
    jumlahAnakLaki = jumlahAnakPerempuan = 0;
    int jumlahIstri = 0;

    for(const auto& waris : daftarWaris) {
        if(waris.hubungan == "Ayah") ayahAda = true;
        else if(waris.hubungan == "Ibu") ibuAda = true;
        else if(waris.hubungan == "Istri") jumlahIstri++;
        else if(waris.hubungan == "Anak Laki-laki") jumlahAnakLaki++;
        else if(waris.hubungan == "Anak Perempuan") jumlahAnakPerempuan++;
    }

    bool punyaAnak = (jumlahAnakLaki > 0 || jumlahAnakPerempuan > 0);
    double sisaHarta = totalHartaWarisan;

    bagianIstri = 0;
    if (jumlahIstri > 0) {
        double totalPorsiIstri = punyaAnak ? (totalHartaWarisan * 0.125) : (totalHartaWarisan * 0.25);
        bagianIstri = totalPorsiIstri / jumlahIstri;
        sisaHarta -= totalPorsiIstri;
    }

    if (ibuAda) {
        bagianIbu = punyaAnak ? (totalHartaWarisan / 6.0) : (totalHartaWarisan / 3.0);
        sisaHarta -= bagianIbu;
    } else bagianIbu = 0;

    porsiPerAnakLaki = porsiPerAnakPerempuan = bagianAyah = 0;

    if (!punyaAnak && ayahAda) {
        bagianAyah = sisaHarta; sisaHarta = 0;
    } else if (punyaAnak) {
        if (ayahAda) { bagianAyah = totalHartaWarisan / 6.0; sisaHarta -= bagianAyah; }
        int totalRasio = (jumlahAnakLaki * 2) + jumlahAnakPerempuan;
        if (totalRasio > 0) {
            double poin = sisaHarta / totalRasio;
            porsiPerAnakLaki = poin * 2;
            porsiPerAnakPerempuan = poin * 1;
        }
    }

    for(auto& waris : daftarWaris) {
        if(waris.hubungan == "Ayah") waris.porsiUang = bagianAyah;
        else if(waris.hubungan == "Ibu") waris.porsiUang = bagianIbu;
        else if(waris.hubungan == "Istri") waris.porsiUang = bagianIstri;
        else if(waris.hubungan == "Anak Laki-laki") waris.porsiUang = porsiPerAnakLaki;
        else if(waris.hubungan == "Anak Perempuan") waris.porsiUang = porsiPerAnakPerempuan;
        else waris.porsiUang = 0;
    }

    simpanSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);
    statusPerhitungan = true;
    statusKlaim = "SIAP DIKLAIM";
    simpanSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);

    cout << "\n[Sukses] Kalkulasi Selesai & Tersimpan ke: " << namaFile << endl;
    cout << "Total Harta: Rp. " << formatRupiah(totalHartaWarisan) << "\n";
    cout << "Tekan Enter untuk kembali..."; cin.get();
}

void eksporSuratNotaris(const string& namaFile) {
    if (!statusPerhitungan) {
        cout << "\n[!] Lakukan kalkulasi pembagian waris terlebih dahulu!\nTekan Enter..."; cin.get(); return;
    }
    string namaPewaris;
    vector<Aset> dAset; vector<AhliWaris> dWaris;
    loadSemuaData(namaFile, namaPewaris, dAset, dWaris);

    string namaHTML = "Akta_Notaris_" + namaPewaris + ".html";
    ofstream file(namaHTML);
    if (file.is_open()) {
        file << "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Surat Ketetapan Notaris</title>";
        file << "<style>body{font-family:'Times New Roman',serif; margin:40px auto; max-width:800px;} h2{text-align:center; border-bottom:2px solid #000; padding-bottom:10px;} table{width:100%; border-collapse:collapse; margin-top:10px;} th,td{border:1px solid #000; padding:10px; text-align:left;} th{background-color:#f2f2f2; font-weight:bold;}</style></head><body>";
        file << "<h2>BERITA ACARA LEGALITAS NOTARIS<br><small>PENETAPAN PEMBAGIAN HARTA WARIS</small></h2>";
        file << "<h3>I. Identitas Pewaris</h3><table><tr><th>Nama Berpulang</th><td><b>" << namaPewaris << "</b></td></tr><tr><th>Total Harta</th><td><b>Rp. " << formatRupiah(totalHartaWarisan) << "</b></td></tr></table>";
        file << "<h3>II. Hak Porsi Ahli Waris</h3><table><tr><th>Nama Keluarga</th><th>Hubungan</th><th>Nominal Porsi Bersih</th></tr>";
        for (const auto& w : dWaris) file << "<tr><td>" << w.nama << "</td><td>" << w.hubungan << "</td><td><b>Rp. " << formatRupiah(w.porsiUang) << "</b></td></tr>";
        file << "</table><p style='text-align:right; margin-top:50px;'>Disahkan oleh Notaris CLI Warisku</p></body></html>";
        file.close();
        cout << "\n[Sukses] Berkas HTML berhasil dicetak: " << namaHTML << "\nBuka file tersebut di browser (Chrome/Safari) lalu tekan Cmd+P untuk Save as PDF!\nTekan Enter...";
    }
    cin.get();
}

void eksporSuratAhliWaris(const string& namaFile) {
    string namaPewaris;
    vector<Aset> dAset; vector<AhliWaris> dWaris;
    loadSemuaData(namaFile, namaPewaris, dAset, dWaris);

    if (dWaris.empty() || (dWaris[0].porsiUang == 0 && dWaris.size() > 0)) {
        cout << "\n[!] Notaris belum mengesahkan nominal pembagian!\nTekan Enter..."; cin.get(); return;
    }
    string namaHTML = "Surat_Ahli_Waris_" + namaPewaris + ".html";
    ofstream file(namaHTML);
    if (file.is_open()) {
        file << "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>SKHW</title>";
        file << "<style>body{font-family:'Times New Roman',serif; margin:40px auto; max-width:800px;} h2{text-align:center; border-bottom:2px solid #000;} table{width:100%; border-collapse:collapse; margin:20px 0;} th,td{border:1px solid #000; padding:10px; text-align:left;} th{background-color:#e6f2ff;}</style></head><body>";
        file << "<h2>SURAT KETERANGAN HAK WARIS (SKHW)</h2><p>Pemberitahuan kepada keluarga Almarhum/ah: <b>" << namaPewaris << "</b>.</p>";
        file << "<table><tr><th>Nama Anggota Keluarga</th><th>Jumlah Dana Diterima</th></tr>";
        for (const auto& w : dWaris) file << "<tr><td>" << w.nama << " (" << w.hubungan << ")</td><td><b>Rp. " << formatRupiah(w.porsiUang) << "</b></td></tr>";
        file << "</table><p>Status: " << statusKlaim << "</p></body></html>";
        file.close();
        cout << "\n[Sukses] Berkas SKHW HTML berhasil dicetak: " << namaHTML << "\nBuka file di browser lalu cetak PDF (Cmd+P)!\nTekan Enter...";
    }
    cin.get();
}

void lihatInformasiPorsiWaris(const string& namaFile) {
    string nPewaris; vector<Aset> dAset; vector<AhliWaris> dWaris;
    loadSemuaData(namaFile, nPewaris, dAset, dWaris);

    if (dWaris.empty() || (dWaris[0].porsiUang == 0 && dWaris.size() > 0)) {
        cout << "\n[!] Notaris belum kalkulasi data harta waris di sistem.\nTekan Enter..."; cin.get(); return;
    }
    system("clear");
    cout << "\n========================================\n       LIHAT PORSI PEMBAGIAN WARIS      \n========================================\n";
    cout << "Pewaris : " << nPewaris << "\n----------------------------------------\n";
    for (size_t i=0; i<dWaris.size(); ++i) cout << (i+1) << ". " << dWaris[i].nama << " (" << dWaris[i].hubungan << ") : Rp. " << formatRupiah(dWaris[i].porsiUang) << "\n";
    cout << "----------------------------------------\nStatus Klaim: [" << statusKlaim << "]\n";
    
    if (statusKlaim == "SIAP DIKLAIM" || statusKlaim == "BELUM DIKLAIM") {
        if (menuInteraktif("Cairkan porsi dana sekarang?", {"Ya, Cairkan", "Nanti Saja"}) == 0) {
            cout << "-> Masukkan Nomor Rekening Bank Anda: "; cin >> nomorRekeningAhliWaris;
            statusKlaim = "MENUNGGU VERIFIKASI NOTARIS"; 
            cout << "\n[Sukses] Pengajuan klaim Anda telah dikirim!\n";
        }
    } else { cout << "Tekan Enter untuk kembali..."; cin.get(); }
}

void menuVerifikasiKlaimNotaris(const string& namaFile) {
    system("clear");
    cout << "\n========================================\n       HALAMAN VERIFIKASI KLAIM        \n========================================\n";
    if (statusKlaim == "MENUNGGU VERIFIKASI NOTARIS") {
        cout << "[Pemberitahuan] Ada 1 Klaim Pencairan!\nRekening: " << nomorRekeningAhliWaris << "\n";
        if (menuInteraktif("Setujui dan cairkan dana waris?", {"Ya, Setujui", "Tangguhkan"}) == 0) {
            statusKlaim = "SELESAI / DICAIRKAN";
            cout << "\n[Sukses] Dana waris resmi dicairkan.\n";
        } else cout << "[Info] Verifikasi klaim ditangguhkan.\n";
    } else cout << "[Data] Status saat ini: " << statusKlaim << "\n";
    cout << "Tekan Enter..."; cin.get();
}