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

    statusPerhitungan = true;
    statusKlaim = "SIAP DIKLAIM";
    simpanSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);

    cout << "\n[Sukses] Kalkulasi Selesai & Tersimpan ke: " << namaFile << endl;
    cout << "Total Harta Warisan Berhasil Dihitung: Rp. " << formatRupiah(totalHartaWarisan) << "\n";
    cout << "Tekan Enter untuk kembali..."; cin.get();
}

void eksporSuratNotaris(const string& namaFile) {
    string namaPewaris;
    vector<Aset> dAset; vector<AhliWaris> dWaris;
    loadSemuaData(namaFile, namaPewaris, dAset, dWaris);

    if (!statusPerhitungan) {
        cout << "\n[!] Lakukan kalkulasi pembagian waris terlebih dahulu!\nTekan Enter..."; cin.get(); return;
    }

    string namaHTML = "Akta_Notaris_" + namaPewaris + ".html";
    ofstream file(namaHTML);
    if (file.is_open()) {
        file << "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Surat Ketetapan Notaris</title>";
        file << "<style>body{font-family:'Times New Roman',serif; margin:50px auto; max-width:850px; line-height:1.6;} .kop{text-align:center; border-bottom:3px double #000; padding-bottom:10px; margin-bottom:20px;} table{width:100%; border-collapse:collapse; margin:15px 0;} th,td{border:1px solid #000; padding:10px; text-align:left;} th{background-color:#f2f2f2; font-weight:bold;}</style></head><body>";
        file << "<div class='kop'><h2>BERITA ACARA LEGALITAS NOTARIS</h2><p>Pejabat Pembuat Akta Waris Terintegrasi CLI | Versi Pro 2026</p></div>";
        file << "<p><b>ID Berkas Kasus:</b> " << namaFile << "<br><b>Status Validasi Hukum:</b> " << statusKlaim << "</p>";
        
        file << "<h3>I. Identitas Pewaris</h3><table><tr><th width='30%'>Nama Pewaris Berpulang</th><td><b>" << namaPewaris << "</b></td></tr><tr><th>Total Nilai Bersih Harta</th><td><b>Rp. " << formatRupiah(totalHartaWarisan) << "</b></td></tr></table>";
        
        file << "<h3>II. Detail Inventarisasi Aset Lengkap</h3><table><tr><th>No</th><th>Kategori</th><th>Komponen Harta Kekayaan</th><th>Informasi / Spesifikasi Detail</th><th>Estimasi Nilai Objek (Rp)</th></tr>";
        for (size_t i=0; i<dAset.size(); ++i) {
            file << "<tr><td>" << (i+1) << "</td><td><b>" << dAset[i].kategori << "</b></td><td>" << dAset[i].nama << "</td><td>" << dAset[i].detailKategori << "</td><td>Rp. " << formatRupiah(dAset[i].nilaiRupiah) << "</td></tr>";
        }
        file << "</table>";

        file << "<h3>III. Distribusi Hak Porsi Ahli Waris</h3><table><tr><th>No</th><th>Nama Ahli Waris</th><th>Tanggal Lahir</th><th>Pekerjaan</th><th>Hubungan Peran Ahli Waris</th><th>Nominal Bagian Bersih</th></tr>";
        for (size_t i=0; i<dWaris.size(); ++i) {
            file << "<tr><td>" << (i+1) << "</td><td>" << dWaris[i].nama << "</td><td>" << dWaris[i].tanggalLahir << "</td><td>" << dWaris[i].pekerjaan << "</td><td>" << dWaris[i].hubungan << "</td><td><b>Rp. " << formatRupiah(dWaris[i].porsiUang) << "</b></td></tr>";
        }
        file << "</table><br><table style='border:none; width:100%;'><tr style='border:none;'><td style='border:none; width:60%;'></td><td style='border:none; text-align:center;'>Yogyakarta, 2026<br><b>Notaris Pengesah Berwenang</b><br><br><br><br>( ____________________ )</td></tr></table></body></html>";
        file.close();
        cout << "\n[Sukses] Berkas Berita Acara Berhasil Diekspor: " << namaHTML << "\nTekan Enter...";
    }
    cin.get();
}

void eksporSuratAhliWaris(const string& namaFile) {
    string namaPewaris;
    vector<Aset> dAset; vector<AhliWaris> dWaris;
    loadSemuaData(namaFile, namaPewaris, dAset, dWaris);

    if (!statusPerhitungan || dWaris.empty()) {
        cout << "\n[!] Notaris belum mengesahkan porsi waris untuk berkas ini!\nTekan Enter..."; cin.get(); return;
    }

    string namaHTML = "Surat_Resmi_Keluarga_" + namaPewaris + ".html";
    ofstream file(namaHTML);
    if (file.is_open()) {
        file << "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Surat Keterangan Hak Waris</title>";
        file << "<style>body{font-family:'Times New Roman',serif; margin:40px auto; max-width:850px; line-height:1.6;} .kop-surat{text-align:center; border-bottom:4px solid #000; padding-bottom:15px; margin-bottom:25px;} .kop-surat h1{margin:0; font-size:24px; text-transform:uppercase;} .kop-surat p{margin:3px 0; font-size:13px; font-style:italic;} table{width:100%; border-collapse:collapse; margin:20px 0;} th,td{border:1px solid #000; padding:12px; text-align:left;} th{background-color:#f8f9fa; font-weight:bold;} .status-box{background-color:#e2ece9; border:1px solid #198754; padding:15px; margin-top:20px; border-radius:5px;}</style></head><body>";
        
        file << "<div class='kop-surat'>";
        file << "<h1>Kantor Pejabat Notaris Pembuat Akta Waris</h1>";
        file << "<p>Jl. Amikom Raya No. 123, Yogyakarta | Telp: (0274) 555-1234 | Fax: (0274) 555-4321</p>";
        file << "<p>Email: official@warisku.notary.id | Website: www.warisku-cli.pro</p>";
        file << "</div>";

        file << "<h3 style='text-align:center; text-decoration:underline;'>SURAT KETERANGAN HAK WARIS (SKHW)</h3>";
        file << "<p>Berdasarkan Ketetapan Hukum Faraidh dan Hukum Perdata Nasional, menerangkan bahwa seluruh rincian harta serta pembagian porsi hak waris dari Almarhum/Almarhumah <b>" << namaPewaris << "</b> dengan total nilai kekayaan objek <b>Rp. " << formatRupiah(totalHartaWarisan) << "</b> adalah sah dan legal menurut hukum:</p>";

        file << "<h3>I. Rincian Objek Kekayaan Harta Waris:</h3>";
        file << "<table><tr><th>No</th><th>Kategori Objek</th><th>Nama Komponen Objek</th><th>Spesifikasi Detail Lapangan</th><th>Estimasi Nilai Objek</th></tr>";
        for (size_t i=0; i<dAset.size(); ++i) {
            file << "<tr><td>" << (i+1) << "</td><td><b>" << dAset[i].kategori << "</b></td><td>" << dAset[i].nama << "</td><td>" << dAset[i].detailKategori << "</td><td>Rp. " << formatRupiah(dAset[i].nilaiRupiah) << "</td></tr>";
        }
        file << "</table>";

        file << "<h3>II. Rincian Pembagian Hak Porsi Ahli Waris Keluarga:</h3>";
        file << "<table><tr><th>Nama Lengkap</th><th>Tanggal Lahir</th><th>Pekerjaan</th><th>Peran Ahli Waris</th><th>Nominal Hak Dana (Rp)</th></tr>";
        for (const auto& w : dWaris) {
            file << "<tr><td>" << w.nama << "</td><td>" << w.tanggalLahir << "</td><td>" << w.pekerjaan << "</td><td>" << w.hubungan << "</td><td><b>Rp. " << formatRupiah(w.porsiUang) << "</b></td></tr>";
        }
        file << "</table>";

        file << "<div class='status-box'><b>INFORMASI SISTEM TRANSFER & KLAIM</b><br>";
        file << "Status Validasi Saat Ini : <b>" << statusKlaim << "</b><br>";
        file << "Nomor Rekening Tujuan     : " << (nomorRekeningAhliWaris.empty() ? "<i>(Belum mendaftarkan nomor rekening)</i>" : nomorRekeningAhliWaris) << "</div>";
        
        file << "<br><p>Surat resmi ini dicetak secara digital untuk dapat dipergunakan sebagaimana mestinya untuk keperluan validasi perbankan.</p>";
        file << "<br><table style='border:none; width:100%;'><tr style='border:none;'><td style='border:none; width:60%;'></td><td style='border:none; text-align:center;'>Disahkan secara Digital oleh:<br><b>Sistem Otomasi Warisku Pro</b></td></tr></table></body></html>";
        file.close();
        cout << "\n[Sukses] Dokumen Surat Resmi Ahli Waris (HTML Ber-KOP) Berhasil Dicetak: " << namaHTML << "\nTekan Enter...";
    }
    cin.get();
}

void lihatInformasiPorsiWaris(const string& namaFile) {
    string nPewaris; vector<Aset> dAset; vector<AhliWaris> dWaris;
    loadSemuaData(namaFile, nPewaris, dAset, dWaris);

    if (!statusPerhitungan || dWaris.empty()) {
        cout << "\n[!] Notaris belum menghitung porsi waris berkas ini.\nTekan Enter..."; cin.get(); return;
    }
    system("clear");
    cout << "\n========================================\n       LIHAT PORSI PEMBAGIAN WARIS      \n========================================\n";
    cout << "Pewaris : " << nPewaris << "\n----------------------------------------\n";
    for (size_t i=0; i<dWaris.size(); ++i) {
        cout << (i+1) << ". " << dWaris[i].nama << " (" << dWaris[i].hubungan << ") : Rp. " << formatRupiah(dWaris[i].porsiUang) << "\n";
    }
    cout << "----------------------------------------\nStatus Klaim Berkas: [" << statusKlaim << "]\n";
    
    if (statusKlaim == "SIAP DIKLAIM" || statusKlaim == "BELUM DIKLAIM") {
        if (menuInteraktif("Cairkan porsi dana sekarang?", {"Ya, Cairkan", "Nanti Saja"}) == 0) {
            cout << "-> Masukkan Nomor Rekening Bank Anda: "; cin >> nomorRekeningAhliWaris;
            cin.ignore(10000, '\n'); // <-- FIX BUG BUFFER SISA ENTER
            statusKlaim = "MENUNGGU VERIFIKASI NOTARIS"; 
            simpanSemuaData(namaFile, nPewaris, dAset, dWaris); 
            cout << "\n[Sukses] Pengajuan klaim Anda telah dikirim!\n";
        }
    } else { cout << "Tekan Enter untuk kembali..."; cin.get(); }
}

void menuVerifikasiKlaimNotaris(const string& namaFile) {
    string nPewaris; vector<Aset> dAset; vector<AhliWaris> dWaris;
    loadSemuaData(namaFile, nPewaris, dAset, dWaris); 

    system("clear");
    cout << "\n========================================\n       HALAMAN VERIFIKASI KLAIM        \n========================================\n";
    if (statusKlaim == "MENUNGGU VERIFIKASI NOTARIS") {
        cout << "[Pemberitahuan] Ada 1 Klaim Pencairan Terdeteksi!\nRekening Ahli Waris: " << nomorRekeningAhliWaris << "\n";
        if (menuInteraktif("Setujui dan cairkan dana waris?", {"Ya, Setujui", "Tangguhkan"}) == 0) {
            statusKlaim = "SELESAI / DICAIRKAN";
            simpanSemuaData(namaFile, nPewaris, dAset, dWaris); 
            cout << "\n[Sukses] Dana waris resmi dicairkan.\n";
        } else cout << "[Info] Verifikasi klaim ditangguhkan.\n";
    } else {
        cout << "[Data] Status Berkas Saat Ini: " << statusKlaim << "\n";
    }
    cout << "Tekan Enter..."; cin.get();
}