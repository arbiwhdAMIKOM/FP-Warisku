#ifndef AUTH_H
#define AUTH_H

#include "models.h"
#include <vector>
#include <string>

// Fungsi database
void loadUsersDariFile(std::vector<User>& daftarUser);
void simpanUserKeFile(const User& userBaru);

// Fungsi fitur
bool prosesLogin(const std::vector<User>& daftarUser, User& userAktif);
void prosesRegister(std::vector<User>& daftarUser);

#endif