#ifndef MODELS_H
#define MODELS_H

#include <string>

struct User {
    std::string username;
    std::string password;
    std::string role; 
};

struct Aset {
    std::string nama;
    double nilaiRupiah;
    std::string kategori;       // "Kendaraan", "Rumah/Tanah", atau "Lainnya"
    std::string detailKategori; // Menampung Brand (Kendaraan) atau Lokasi & Luas (Rumah/Tanah)
};

struct AhliWaris {
    std::string nama;
    std::string hubungan; 
    double porsiUang;
    bool isKlaim;
    std::string tanggalLahir; // Format wajib: DD-MM-YYYY
    std::string pekerjaan;    
};

#endif