#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <sstream>

#include "../include/filehandle.h"

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

using namespace std;

filehandle::filehandle(const char* logfiles_path): logfiles_path(logfiles_path) {
    //for (int i = 0; i < num_paths; i++) {
    //    this->logfile_paths[i] = logfile_paths[i];
    // }
}

filehandle::filehandle(){}

string filehandle::get_filename() {
    auto t = time(nullptr);
    auto tm = *localtime(&t);

    ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    auto date = oss.str();
    string string = logfiles_path;
    string += date + ".csv";
    return string;
}

/*void filehandle::write_err_line(string line, fstream& file) {
    if (file.is_open()) {
        auto t = time(nullptr);
        auto tm = *localtime(&t);
        ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %T");
        auto date = oss.str();
        file << date << "   " << line << endl;
    }
}*/

void filehandle::write_line(string line, fstream& file) {
    if (file.is_open()) {
        file << line << endl;
        // file.close();
    }
}

void filehandle::init_file(fstream& file) {
    string first_line;
    getline(file, first_line);
    file.close();
    file.clear();
    // check if file content exists, if not write first line to file
    if (first_line.length() <= 1) {
        file.open(get_filename());
        write_line("Alter, PLZ, Frage 1, Frage 2, Frage 3, Frage 4, Frage 5, Frage 6, Frage 7, Frage 8, Frage 9 ", file);
    }
    else {
        // reopen filestream for further writing
        file.open(get_filename(), fstream::out | fstream::app);
    }
}