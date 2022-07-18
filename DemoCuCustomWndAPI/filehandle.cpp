#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <sstream>

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

using namespace std;

std::string get_filename() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y");
    auto date = oss.str();
    string string = "C:\\235Media\\";
    string += date + ".csv";
    return string;
}

void write_line(std::string line, fstream& file) {
    if (file.is_open()) {
        file << line << std::endl;
        // file.close();
    }
}

void init_file(fstream& file) {
    std::string first_line;
    getline(file, first_line);
    file.close();
    if (first_line.length() <= 1) {
        file.open(get_filename());
        write_line("Alter, PLZ, Frage 1, Frage 2, Frage 3, Frage 4, Frage 5, Frage 6, Frage 7, Frage 8, Frage 9 ", file);
    }
}