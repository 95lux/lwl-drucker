#include <Windows.h>
#include <iostream>

class serial_device {
public:
	HANDLE h_Serial;
	COMMTIMEOUTS timeout = { 0 };
	DCB dcbSerialParam = { 0 };

	void open_serial_port(LPCWSTR);
	void init_serial_port(void);
	void config_timeout(void);
	bool read_line(char* str_out);
	bool read_rfid(int* alter, int* plz, int(&answers_arr)[9]);
	serial_device(LPCWSTR);

};


