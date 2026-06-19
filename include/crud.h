#ifndef CRUD_H
#define CRUD_H

#include "models.h"
#include <vector>
#include <string>

// Fungsi pembantu
std::string formatRupiah(double nilai);

// Menu CRUD diatur di sini dengan parameter
void menuCRUDRahmat(const std::string& namaFile);

// Fungsi database membaca & menulis (Dinamis berdasarkan nama file kasus)
void loadSemuaData(const std::string& namaFile, std::string& namaPewaris, std::vector<Aset>& daftarAset, std::vector<AhliWaris>& daftarWaris);
void simpanSemuaData(const std::string& namaFile, const std::string& namaPewaris, const std::vector<Aset>& daftarAset, const std::vector<AhliWaris>& daftarWaris);

// Fungsi Utama Fitur Waris
void prosesInputSistemWarisan(const std::string& namaFile, std::string& namaPewaris, std::vector<Aset>& daftarAset, std::vector<AhliWaris>& daftarWaris);
void tampilkanRingkasanData(const std::string& namaPewaris, const std::vector<Aset>& daftarAset, const std::vector<AhliWaris>& daftarWaris);
void ubahData(const std::string& namaFile, std::string& namaPewaris, std::vector<Aset>& daftarAset, std::vector<AhliWaris>& daftarWaris);
void hapusData(const std::string& namaFile, std::string& namaPewaris, std::vector<Aset>& daftarAset, std::vector<AhliWaris>& daftarWaris);

#endif