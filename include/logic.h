#ifndef LOGIC_H
#define LOGIC_H

#include <string>

extern double totalHartaWarisan;
extern double bagianIstri;
extern double bagianAyah;
extern double bagianIbu;
extern int jumlahAnakLaki;
extern int jumlahAnakPerempuan;
extern bool ayahAda;
extern bool ibuAda;
extern double porsiPerAnakLaki;
extern double porsiPerAnakPerempuan;
extern bool statusPerhitungan;
extern std::string statusKlaim;
extern std::string nomorRekeningAhliWaris;

// Semua fungsi sekarang minta nama file
void menuKalkulatorWaris(const std::string& namaFile);
void lihatInformasiPorsiWaris(const std::string& namaFile);
void menuVerifikasiKlaimNotaris(const std::string& namaFile); 
void eksporSuratNotaris(const std::string& namaFile);
void eksporSuratAhliWaris(const std::string& namaFile);

std::string formatRupiah(double angka);

#endif