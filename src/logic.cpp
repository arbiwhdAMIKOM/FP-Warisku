#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include "../include/logic.h"
#include "../include/models.h"
#include "../include/crud.h"

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

void clearScr();
void jeda();

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

// FUNGSI 1: Kalkulator Pembagian (Oleh Notaris)
void menuKalkulatorWaris() {
    string namaFile = "data_waris.txt";
    string namaPewaris;
    vector<Aset> daftarAset;
    vector<AhliWaris> daftarWaris;

    cout << "\n========================================" << endl;
    cout << "       KALKULATOR PEMBAGIAN WARIS       " << endl;
    cout << "========================================" << endl;
    cout << " [Proses] Membaca data dari database...\n";

    loadSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);

    if(daftarAset.empty() || daftarWaris.empty()) {
        cout << " [!] Gagal: Data Aset atau Anggota Keluarga belum diisi.\n";
        cout << "     Silakan isi terlebih dahulu di Menu Kelola Data (Menu 1).\n";
        jeda();
        return;
    }

    totalHartaWarisan = 0;
    for(const auto& aset : daftarAset) {
        totalHartaWarisan += aset.nilaiRupiah;
    }

    ayahAda = false;
    ibuAda = false;
    jumlahAnakLaki = 0;
    jumlahAnakPerempuan = 0;
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
        bagianIbu = punyaAnak ? (totalHartaWarisan / 6.0) : (totalHartaWarisan / 3.0); // <-- PERBAIKAN: Spasi dihapus menjadi bagianIbu
        sisaHarta -= bagianIbu;
    } else {
        bagianIbu = 0;
    }

    porsiPerAnakLaki = 0;
    porsiPerAnakPerempuan = 0;
    bagianAyah = 0;

    if (!punyaAnak) {
        if (ayahAda) {
            bagianAyah = sisaHarta;
            sisaHarta = 0;
        }
    } else {
        if (ayahAda) {
            bagianAyah = totalHartaWarisan / 6.0;
            sisaHarta -= bagianAyah;
        }

        int totalPoinRasio = (jumlahAnakLaki * 2) + (jumlahAnakPerempuan * 1);
        if (totalPoinRasio > 0) {
            double nilaiPerSatuPoin = sisaHarta / totalPoinRasio;
            porsiPerAnakLaki = nilaiPerSatuPoin * 2;
            porsiPerAnakPerempuan = nilaiPerSatuPoin * 1;
            sisaHarta = 0;
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

    cout << "\n[Sukses] Perhitungan Selesai dan Berhasil Disimpan ke Berkas!" << endl;
    cout << "-> Total Harta Warisan        : Rp. " << formatRupiah(totalHartaWarisan) << endl;
    cout << "-> Porsi Istri                : Rp. " << formatRupiah(bagianIstri) << endl;
    if (ibuAda)  cout << "-> Porsi Ibu                  : Rp. " << formatRupiah(bagianIbu) << endl;
    if (ayahAda) cout << "-> Porsi Ayah                 : Rp. " << formatRupiah(bagianAyah) << endl;
    if (jumlahAnakLaki > 0) cout << "-> Porsi per Anak Laki        : Rp. " << formatRupiah(porsiPerAnakLaki) << endl;
    if (jumlahAnakPerempuan > 0) cout << "-> Porsi per Anak Perempuan   : Rp. " << formatRupiah(porsiPerAnakPerempuan) << endl;
    cout << "========================================" << endl;
    cout << "\n[Info] Untuk mengunduh surat ketetapan resmi, silakan pilih menu nomor 3.\n";
    jeda();
}

// FUNGSI EKSPOR 1: SURAT RESMI KETETAPAN NOTARIS (GRID CLEAN LAYOUT)
void eksporSuratNotaris() {
    if (!statusPerhitungan) {
        cout << "\n[!] Gagal: Lakukan kalkulasi pembagian waris terlebih dahulu (Menu 2)!\n";
        jeda();
        return;
    }

    string namaFile = "data_waris.txt";
    string namaPewaris;
    vector<Aset> daftarAset;
    vector<AhliWaris> daftarWaris;
    loadSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);

    ofstream file("surat_resmi_notaris.csv");
    if (file.is_open()) {
        // HEAD BANNER DOKUMEN
        file << "===================================================================================================\n";
        file << "BERITA ACARA LEGALITAS NOTARIS,,PENETAPAN RESMI PEMBAGIAN HARTA WARIS\n";
        file << "===================================================================================================\n";
        file << "Nomor SK Ketetapan,,WRS/NTRS/2026/001\n";
        file << "Hari / Tanggal Penetapan,,Selasa / 16 Juni 2026\n";
        file << "Institusi Pengesah,,Kantor Pejabat Pembuat Akta Waris (Warisku CLI)\n";
        file << "---------------------------------------------------------------------------------------------------\n\n";

        // KATEGORI 1
        file << "I. IDENTITAS PEWARIS & AKUMULASI HARTA\n";
        file << "---------------------------------------------------------------------------------------------------\n";
        file << "Nama Pewaris Berpulang,,\"" << namaPewaris << "\"\n";
        file << "Total Nilai Harta Bersih,,Rp. " << formatRupiah(totalHartaWarisan) << "\n\n";

        // KATEGORI 2
        file << "II. RINCIAN INVENTARISASI DATA ASET\n";
        file << "---------------------------------------------------------------------------------------------------\n";
        file << "No,Nama Komponen Harta Kekayaan,,Estimasi Nilai Rupiah\n";
        for (size_t i = 0; i < daftarAset.size(); ++i) {
            file << (i+1) << "," << daftarAset[i].nama << ",,Rp. " << formatRupiah(daftarAset[i].nilaiRupiah) << "\n";
        }
        file << "\n";

        // KATEGORI 3
        file << "III. DAFTAR HAK PORSI AHLI WARIS SAH (HUKUM FARAIDH)\n";
        file << "---------------------------------------------------------------------------------------------------\n";
        file << "No,Nama Lengkap Anggota Keluarga,Hubungan Darah,Nominal Porsi Bagian Bersih\n";
        for (size_t i = 0; i < daftarWaris.size(); ++i) {
            file << (i+1) << "," << daftarWaris[i].nama << "," << daftarWaris[i].hubungan << ",Rp. " << formatRupiah(daftarWaris[i].porsiUang) << "\n";
        }
        file << "\n";

        // KATEGORI 4
        file << "IV. STATUS HUKUM FINAL DAN VERIFIKASI DANA\n";
        file << "---------------------------------------------------------------------------------------------------\n";
        file << "Status Validasi Dokumen,,\"" << statusKlaim << "\"\n";
        file << "Nomor Rekening Pencairan Terpaut,,\"" << (nomorRekeningAhliWaris.empty() ? "-" : nomorRekeningAhliWaris) << "\"\n\n";

        // FOOTER TANDA TANGAN
        file << "---------------------------------------------------------------------------------------------------\n";
        file << "Dokumen Hukum ini diterbitkan dan disahkan secara digital oleh Notaris yang berwenang.\n";
        file << "Sistem Otomasi Manajemen Akta Warisku Versi Pro CLI v2.0 - 2026\n";

        file.close();
        cout << "\n[Sukses] Berhasil mengekspor berkas akta resmi: 'surat_resmi_notaris.csv'!\n";
    } else {
        cout << "\n[!] Gagal membuat berkas akta resmi.\n";
    }
    jeda();
}

// FUNGSI EKSPOR 2: SURAT KETERANGAN AHLI WARIS (GRID CLEAN LAYOUT)
void eksporSuratAhliWaris() {
    string namaFile = "data_waris.txt";
    string namaPewaris;
    vector<Aset> daftarAset;
    vector<AhliWaris> daftarWaris;
    loadSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);

    if (daftarWaris.empty() || (daftarWaris[0].porsiUang == 0 && daftarWaris.size() > 0)) {
        cout << "\n[!] Gagal: Notaris belum mengesahkan nominal pembagian porsi waris Anda!\n";
        jeda();
        return;
    }

    ofstream file("surat_keterangan_ahli_waris.csv");
    if (file.is_open()) {
        // HEAD BANNER DOKUMEN FAMILY
        file << "===================================================================================================\n";
        file << "SURAT KETERANGAN HAK WARIS (SKHW),,SALINAN RESMI KELUARGA\n";
        file << "===================================================================================================\n";
        file << "Diterbitkan Oleh,,Pejabat Kantor Notaris Warisku CLI\n";
        file << "Perihal,,Keterangan Hak Porsi Dana dan Status Pencairan\n";
        file << "---------------------------------------------------------------------------------------------------\n\n";

        file << "Pemberitahuan Resmi Kepada Seluruh Ahli Waris Almarhum/Almarhumah: " << namaPewaris << "\n\n";
        file << "Berdasarkan amar putusan ketetapan hukum waris yang telah diverifikasi oleh Notaris,\n";
        file << "berikut adalah rincian porsi dana tunai bersih yang mutlak menjadi hak milik Anda:\n\n";

        // TABEL PORSI
        file << "DAFTAR PENERIMA MANFAAT DAN PORSI DANA\n";
        file << "---------------------------------------------------------------------------------------------------\n";
        file << "Nama Lengkap Keluarga,Hubungan Ahli Waris,Jumlah Dana Bersih Diterima\n";
        for (const auto& waris : daftarWaris) {
            file << waris.nama << "," << waris.hubungan << ",Rp. " << formatRupiah(waris.porsiUang) << "\n";
        }
        file << "\n";

        // DETAIL VALIDASI REKENING
        file << "INFORMASI STATUS KLAIM DAN SINKRONISASI TRANSFER BANK\n";
        file << "---------------------------------------------------------------------------------------------------\n";
        file << "Status Berkas Saat Ini,,\"" << statusKlaim << "\"\n";
        file << "Nomor Rekening Penerima Manfaat,,\"" << (nomorRekeningAhliWaris.empty() ? "(Belum Mendaftarkan Rekening)" : nomorRekeningAhliWaris) << "\"\n\n";

        // KETENTUAN FOOTER
        file << "---------------------------------------------------------------------------------------------------\n";
        file << "Catatan Penting:\n";
        file << "1. Surat Keterangan ini merupakan salinan bukti otentik yang sah bagi pihak keluarga.\n";
        file << "2. Pencairan dana menuju rekening bank akan diproses sepenuhnya jika status berkas telah SELESAI.\n";

        file.close();
        cout << "\n[Sukses] Berhasil mengunduh dokumen resmi keluarga: 'surat_keterangan_ahli_waris.csv'!\n";
    } else {
        cout << "\n[!] Gagal mengunduh berkas surat keterangan.\n";
    }
    jeda();
}

// Lihat Informasi & Ajukan Klaim (Oleh Ahli Waris)
void lihatInformasiPorsiWaris() {
    string namaFile = "data_waris.txt";
    string namaPewaris;
    vector<Aset> daftarAset;
    vector<AhliWaris> daftarWaris;

    loadSemuaData(namaFile, namaPewaris, daftarAset, daftarWaris);

    if (daftarWaris.empty() || (daftarWaris[0].porsiUang == 0 && daftarWaris.size() > 0)) {
        cout << "\n[!] Notaris belum melakukan kalkulasi data harta waris di sistem.\n";
        jeda();
        return;
    }

    totalHartaWarisan = 0;
    for (const auto& aset : daftarAset) {
        totalHartaWarisan += aset.nilaiRupiah;
    }

    clearScr();
    cout << "\n========================================" << endl;
    cout << "       LIHAT PORSI PEMBAGIAN WARIS      " << endl;
    cout << "========================================" << endl;
    cout << "Nama Pewaris                 : " << namaPewaris << endl;
    cout << "Total Harta Warisan Keluarga : Rp. " << formatRupiah(totalHartaWarisan) << endl;
    cout << "----------------------------------------" << endl;
    cout << "Rincian Hak Waris Terdaftar di Berkas:\n";
    
    for (size_t i = 0; i < daftarWaris.size(); ++i) {
        cout << (i + 1) << ". Porsi " << left << setw(15) << daftarWaris[i].nama 
             << " (" << setw(14) << daftarWaris[i].hubungan << ") : Rp. " 
             << formatRupiah(daftarWaris[i].porsiUang) << endl;
    }
    
    cout << "----------------------------------------" << endl;
    cout << "Status Klaim Dana Keluarga: [" << statusKlaim << "]\n"; 
    cout << "========================================" << endl;

    if (statusKlaim == "SIAP DIKLAIM" || statusKlaim == "BELUM DIKLAIM") {
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
    jeda();
}

// Menu Verifikasi Persetujuan Klaim (Oleh Notaris)
void menuVerifikasiKlaimNotaris() {
    cout << "\n========================================" << endl;
    cout << "       HALAMAN VERIFIKASI KLAIM        " << endl;
    cout << "========================================" << endl;
    if (statusKlaim == "BELUM DIKLAIM" || statusKlaim == "SIAP DIKLAIM") {
        cout << "[Data] Belum ada berkas pengajuan pencairan dari Ahli Waris.\n";
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
    jeda();
}