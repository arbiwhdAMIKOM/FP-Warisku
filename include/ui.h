#ifndef UI_H
#define UI_H

#include <iostream>
#include <vector>
#include <string>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
inline int _getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

using namespace std;

// Fungsi Menu Interaktif Keren
inline int menuInteraktif(const string& headerJudul, const vector<string>& listMenu) {
    int pilihan = 0;
    while (true) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        cout << "\033[1;36m========================================\033[0m\n";
        cout << "\033[1m  " << headerJudul << "\033[0m\n";
        cout << "\033[1;36m========================================\033[0m\n\n";

        for (size_t i = 0; i < listMenu.size(); ++i) {
            if (i == pilihan) {
                cout << "\033[1;32m  > [ " << listMenu[i] << " ] < \033[0m\n";
            } else {
                cout << "      " << listMenu[i] << "   \n";
            }
        }
        cout << "\n\033[90m(Gunakan panah Atas/Bawah dan Enter)\033[0m\n";

        int key = _getch();
        if (key == 224) { 
            key = _getch();
            if (key == 72) pilihan = (pilihan > 0) ? pilihan - 1 : listMenu.size() - 1; 
            else if (key == 80) pilihan = (pilihan < listMenu.size() - 1) ? pilihan + 1 : 0; 
        } 
        else if (key == 27) { 
            key = _getch();
            if (key == 91) { 
                key = _getch();
                if (key == 65) pilihan = (pilihan > 0) ? pilihan - 1 : listMenu.size() - 1; 
                else if (key == 66) pilihan = (pilihan < listMenu.size() - 1) ? pilihan + 1 : 0; 
            }
        } 
        else if (key == '\n' || key == '\r') { 
            return pilihan; 
        }
    }
}

// Fungsi Input Live (Otomatis Kasih Titik Saat Ngetik)
inline double inputRupiahLive() {
    string input = "";
    while (true) {
        cout << "\r       Nilai (Rp): ";
        string formatted = "";
        int count = 0;
        for (int i = (int)input.length() - 1; i >= 0; i--) {
            formatted = input[i] + formatted;
            count++;
            if (count % 3 == 0 && i != 0) formatted = '.' + formatted;
        }
        cout << formatted << " \033[K"; // Bersihkan sisa karakter di kanan

        int key = _getch();
        if (key == '\n' || key == '\r') {
            cout << endl;
            if (input.empty()) return 0;
            return stod(input);
        } else if (key == 127 || key == 8) { // Tombol Delete/Backspace
            if (!input.empty()) input.pop_back();
        } else if (key >= '0' && key <= '9') {
            if (input.length() < 15) input += (char)key; // Batas max agar tidak crash
        }
    }
}

#endif