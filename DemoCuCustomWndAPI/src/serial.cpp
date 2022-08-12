#include "stdafx.h"
#include "../include/serial.h"
#include "../include/json.h"
#include "../include/answers.h"

#include <algorithm>

serial_device::serial_device(LPCWSTR comport) {
    open_serial_port(comport);
    init_serial_port();

}

void serial_device::open_serial_port(LPCWSTR comport){
        h_Serial = CreateFile(comport, GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);
    if (h_Serial == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            //serial port not found. Handle error here.
            std::cout << "[err] Couldn't open rfid reader on "<< comport <<"! RFID Reader not attached!" << std::endl;
        }
        //any other error. Handle error here.
        std::cout << "[err] Couldn't open rfid reader on "<< comport << "! Unspecified Error!" << std::endl;
    }
}

void serial_device::init_serial_port(){
    dcbSerialParam.DCBlength = sizeof(dcbSerialParam);

    if (!GetCommState(h_Serial, &dcbSerialParam)) {
        //handle error here
    }

    dcbSerialParam.BaudRate = CBR_115200;
    dcbSerialParam.ByteSize = 8;
    dcbSerialParam.StopBits = ONESTOPBIT;
    dcbSerialParam.Parity = NOPARITY;

    if (!SetCommState(h_Serial, &dcbSerialParam)) {
        //handle error here
    }
   
}

void serial_device::config_timeout() {
    timeout.ReadIntervalTimeout = 60;
    timeout.ReadTotalTimeoutConstant = 60;
    timeout.ReadTotalTimeoutMultiplier = 15;
    timeout.WriteTotalTimeoutConstant = 60;
    timeout.WriteTotalTimeoutMultiplier = 8;
    if (!SetCommTimeouts(h_Serial, &timeout)) {
        //handle error here
    }
}

bool serial_device::read_line(char *str_out) {
    const int n = 32;
    int i = 0;
    char buf[n] = { 0 };
    DWORD read_bytes = 0;
    
    while (true) {
        if (ReadFile(h_Serial, buf, 1, &read_bytes, NULL)) {
            if ((read_bytes > 0) && (buf[0] != '\n')) {
                str_out[i] = buf[0];
                i++;
            } else if(buf[0] == '\n') {
                str_out[i-1] = '\0';
                str_out[i] = '\0';
                return true;
            }
        }
        else {
            if (!ReadFile(h_Serial, buf, 1, &read_bytes, NULL)) {
                std::cout << "[err] Retrieve line from RFID reader failed" << std::endl;
            }
            if (i > n) {
                std::cout << "[err] Serial Port char Buffer filled! " << std::endl;
            }
            return false;
        }
    }
}

bool serial_device::read_rfid(int* alter, int* plz, int (&answers_arr)[9]) {
    char str[256] = "";
    read_line(str);
    // char *str = "{'a4': '0', 'a5': '0', 'a6': '0', 'a7': '0', 'a8': '2', 'a9': '500', 'a1': '1', 'a2': '0', 'a3': '0', 'printed': '1', 'age': '0', 'plz': '4'}";
    std::string raw_json = str;
    std::replace(raw_json.begin(), raw_json.end(), '\'', '"');
    const auto raw_json_length = static_cast<int>(raw_json.length());
    JSONCPP_STRING err;
    Json::Value root;

    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(raw_json.c_str(), raw_json.c_str() + raw_json_length, &root,
        &err)) {
        std::cout << "[err] Corrupt JSON format from RFID reader!" << std::endl;
        std::cout << raw_json.c_str() << std::endl;
        return false;
    }

    // convert key value from string to int. reading value as int directly throws json logic excpetion
    if (root["age"].asString().empty()) {
        std::cout << "[err] Empty JSON value from key \"age\"" << std::endl;
        std::cout << raw_json.c_str() << std::endl;
        *alter = 0;
    }
    else {
        *alter = std::stoi(root["age"].asString());

    }
    if (root["plz"].asString().empty()) {
        std::cout << "[err] Empty JSON value from key \"plz\"" << std::endl;
        std::cout << raw_json.c_str() << std::endl;
        *plz = 0;
    }
    else {
        *plz = std::stoi(root["plz"].asString());
    }

    for (int i = 0; i < 9; i++) {
        
        std::string str_json = std::to_string(i + 1);
        std::string str = "a";
        str += str_json;

        if (root[str].asString().empty()) {
            std::cout << "[err] Empty JSON value from key "<< str << std::endl;
            std::cout << raw_json.c_str() << std::endl;
            // return false;
            answers_arr[i] = 0;
        }
        else {
            answers_arr[i] = std::stoi(root[str].asString());
        }
        
    }
    std::cout << "[!] Successfully read RFID card!" << std::endl;
    return true;
}