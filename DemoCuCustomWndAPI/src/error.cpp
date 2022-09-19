#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <sstream>

#include "../include/error.h"

/*void write_err_line(string line, fstream& file) {
    if (file.is_open()) {
        auto t = time(nullptr);
        auto tm = *localtime(&t);
        ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %T");
        auto date = oss.str();
        file << date << "   " << line << endl;
    }
}
*/