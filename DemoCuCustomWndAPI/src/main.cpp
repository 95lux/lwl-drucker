#include "stdafx.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>  
#include <time.h>
#include <iostream>
#include <fstream>

#include "../include/answers.h"
#include "../include/drucker.h"
#include "../include/filehandle.h"
#include "../include/serial.h"
#include "../include/SimpleIni.h"

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#define ENABLE_PRINTER

using namespace std;

int get_rndm_num() {
	int num = (rand() % (4 - 1 + 1)) + 1;
	return num;
}

const wchar_t* get_wc(const char* c) {
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

int _tmain(int argc, _TCHAR* argv[]) {
	// import config.ini
	CSimpleIniA ini;
	ini.SetUnicode();
	SI_Error rc = ini.LoadFile("C:\\235Media\\lwldrucker\\config.ini");
	if (rc < 0) {
		std::cout << "[err] config.ini not found at 'C:\\235Media\\lwldrucker\\config.ini' Exiting program. " << std::endl;
		return 0;
	};

	const char* logo_path = ini.GetValue("path", "logo_path", "default");
	const char* logfiles_path = ini.GetValue("path", "logfiles_path", "default");
	const char* printer_com = ini.GetValue("com", "printer_com", "default");
	const char* rfid_com = ini.GetValue("com", "rfid_com", "default");

	// Antworkmaste
	#define ja_aber_woanders			1
	#define naja_mir_egal				2
	#define ja_weniger_ist_mehr			3
	#define ja_aber_veränderung_nein	4

	int answers_mask[9][4] =
	{
		{ja_weniger_ist_mehr		,ja_aber_veränderung_nein	,ja_aber_woanders					,naja_mir_egal				},
		{ja_weniger_ist_mehr		,naja_mir_egal				,ja_aber_veränderung_nein			,ja_aber_woanders			},
		{ja_weniger_ist_mehr		,ja_aber_veränderung_nein	,ja_aber_woanders					,naja_mir_egal				},
		{ja_weniger_ist_mehr		,naja_mir_egal				,ja_aber_veränderung_nein			,ja_aber_woanders			},
		{ja_aber_veränderung_nein	,ja_aber_woanders			,naja_mir_egal						,ja_weniger_ist_mehr		},
		{naja_mir_egal				,ja_aber_woanders			,ja_weniger_ist_mehr				,ja_aber_veränderung_nein	},
		{ja_aber_woanders			,naja_mir_egal				,ja_weniger_ist_mehr				,ja_aber_veränderung_nein	},
		{naja_mir_egal				,ja_aber_veränderung_nein	,ja_aber_woanders					,ja_weniger_ist_mehr		}
	};

	// init RFID reader
	char str_buf[32] = "";
	serial_device *rfid_reader = new serial_device(get_wc(rfid_com));
	
#ifdef ENABLE_PRINTER	
	initPrinter(get_wc(printer_com));

	PrintFontStruct pfs = PRINTFONTSTRUCT_INIT;
	pfs.charWidth = FONT_SIZE_X1;
	pfs.charHeight = FONT_SIZE_X1;
	pfs.justification = FONT_JUSTIFICATION_LEFT;
	pfs.bLineSpacing = 100;
	pfs.emphasized = TRUE;
	pfs.italic = FALSE;
	pfs.leftMarginValue = 50;
#endif
	//char* linebuffer[32];

	 // create filehandling infrastructure  
	filehandle *fhandle = new filehandle(logfiles_path);
	string filename = fhandle->get_filename();
	fstream filestream(filename, fstream::in | fstream::out | fstream::app);
	fhandle->init_file(filestream);
	string last_line = "";

	while (true){

		// int answers_array[] = { get_rndm_num(),get_rndm_num(),get_rndm_num(),get_rndm_num(),get_rndm_num(),get_rndm_num(),get_rndm_num(),get_rndm_num(),get_rndm_num() };
		int alter, plz;
		int answers_arr[9];
		// only carry on processing on correct data read
		if (rfid_reader->read_rfid(&alter, &plz, answers_arr)) {
			answers answersUser(alter, plz, answers_arr, answers_mask);
			if (last_line.compare(answersUser.line) != 0) {
				fhandle->write_line(answersUser.line, filestream);
				last_line = answersUser.line;

#ifdef ENABLE_PRINTER

				// Print Image
				DoPrintImage(get_wc(logo_path));

				// Typ 1
				pfs.bLineSpacing = 50;
				DoPrintLine(L"Ja, aber woanders.", pfs);
				pfs.bLineSpacing = 60;
				answersUser.print_question(0, pfs);

				// Typ 2
				pfs.bLineSpacing = 50;
				DoPrintLine(L"Na ja, mir egal.", pfs);
				pfs.bLineSpacing = 60;

				answersUser.print_question(1, pfs);

				// Typ 3
				pfs.bLineSpacing = 50;
				DoPrintLine(L"Ja, weniger ist mehr.", pfs);
				pfs.bLineSpacing = 60;

				answersUser.print_question(2, pfs);

				// Typ 4
				pfs.bLineSpacing = 50;
				DoPrintLine(L"Ja, aber Veränderung nein.", pfs);
				pfs.bLineSpacing = 60;

				answersUser.print_question(3, pfs);

				pfs.bLineSpacing = 50;
				DoPrintLine(L"", pfs);
				DoPrintLine(L"Im Film erfährst du mehr.", pfs);
				DoPrintLine(L"Welcher Weg entspricht dir?", pfs);
				DoPrintLine(L"Entscheide dich!", pfs);

				DoPrintBarcode();

				DoCut();
#endif
			}
		}
	}
#ifdef ENABLE_PRINTER
	// DeInit library
	deInitPrinter();
#endif
	return EXIT_SUCCESS;
}