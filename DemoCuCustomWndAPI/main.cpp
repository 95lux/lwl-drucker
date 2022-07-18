﻿#include "stdafx.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>  
#include <time.h>
#include <iostream>
#include <fstream>

#include "answers.h"
#include "drucker.h"
#include "filehandle.h"
#include "serial.h"

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1

int get_rndm_num() {
	int num = (rand() % (4 - 1 + 1)) + 1;
	return num;
}

int _tmain(int argc, _TCHAR* argv[])
{
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

	char str_buf[32] = "";
	serial_device *rfid_reader = new serial_device(L"COM4");
	
#ifdef ENABLE_PRINTER	
	initPrinter();

	PrintFontStruct pfs = PRINTFONTSTRUCT_INIT;
	pfs.charWidth = FONT_SIZE_X1;
	pfs.charHeight = FONT_SIZE_X1;
	pfs.justification = FONT_JUSTIFICATION_LEFT;
	pfs.bLineSpacing = 100;
	pfs.emphasized = TRUE;
	pfs.italic = FALSE;
	pfs.leftMarginValue = 50;
#endif
	char* linebuffer[32];
	// std::fstream filestream;
	std::string filename = get_filename();
	std::fstream filestream(filename, std::fstream::in | std::fstream::out | std::fstream::app);
	init_file(filestream);
	// open_file(filestream);

	while (true){

		int answers_array[] = { get_rndm_num(),get_rndm_num(),get_rndm_num(),get_rndm_num(),get_rndm_num(),get_rndm_num(),get_rndm_num(),get_rndm_num(),get_rndm_num() };
		int alter, plz;
		int answers_arr[9];
		rfid_reader->read_rfid(&alter, &plz, answers_arr);

		answers answersUser(alter, plz, answers_arr, answers_mask);
		write_line(answersUser.line, filestream);
		std::cout << "Press ENTER to print sample!" << std::endl;
		getchar();

		// Print Image
		DoPrintImage(L"C:\\235Media\\logo.jpg");

		wchar_t buf_int[30];
		wchar_t buf_str[30];

		/*
		// Alter
		swprintf(buf_int, sizeof buf_int, L"%d", answersUser1->alter);
		wcscpy(buf_str, L"Alter: ");
		DoPrintLine(wcscat(buf_str, buf_int), pfs);

		// PLZ
		swprintf(buf_int, sizeof buf_int, L"%d", answersUser1->plz);
		wcscpy(buf_str, L"PLZ:   ");
		DoPrintLine(wcscat(buf_str, buf_int), pfs);
		*/

#ifdef ENABLE_PRINTER
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
	
#ifdef ENABLE_PRINTER
	// DeInit library
	deInitPrinter();
#endif
	return EXIT_SUCCESS;
}