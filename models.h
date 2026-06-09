#ifndef MODELS_H
#define MODELS_H

#include <string>
using namespace std;

// fitur Login (Auth)
struct User {
    string username;
    string password;
    string role; // "Notaris" atau "AhliWaris"
};

struct Aset {
    string nama;
    double nilaiRupiah;
};

struct AhliWaris {
    std::string nama;
    std::string hubungan;
    double porsiUang;
    bool isKlaim;
};

#endif