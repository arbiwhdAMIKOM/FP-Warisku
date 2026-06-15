#ifndef LOGIC_H
#define LOGIC_H

#include <string>

extern double totalHartaWarisan;
extern double bagianIstri;
extern double bagianAyah;
extern double bagianIbu;

// Variabel Kontrak untuk Tim CRUD
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
void eksporKeCSV();
std::string formatRupiah(double angka);

#endif