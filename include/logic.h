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

void menuKalkulatorWaris();
void lihatInformasiPorsiWaris();
void menuVerifikasiKlaimNotaris(); 

// Dua fungsi ekspor surat resmi yang terpisah
void eksporSuratNotaris();
void eksporSuratAhliWaris();

std::string formatRupiah(double angka);

#endif