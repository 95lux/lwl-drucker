﻿#include "stdafx.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>  
#include <time.h>
#include <iostream>

#include "answers.h"
#include "drucker.h"
#include "filehandle.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int answers_mask[8][4] =
	{
		{1,2,3,4},
		{4,3,2,1},
		{1,1,2,2},
		{2,2,1,1},
		{4,2,3,1},
		{2,2,2,1},
		{1,2,3,2},
		{4,4,1,1}
	};

	int answers_array[] = { 1,2,3,4,1,2,3,4 };
	answers *answersUser1 = new answers(18, 52835, answers_array, answers_mask);
	char* s1 = answersUser1->get_bar(0);
 	std::cout << s1 << std::endl;
	std::cout << answersUser1->get_bar(1) << std::endl;
	std::cout << answersUser1->get_bar(2) << std::endl;
	std::cout << answersUser1->get_bar(3) << std::endl;

	char* linebuffer[32];

	PrintFontStruct pfs = PRINTFONTSTRUCT_INIT;
	pfs.charWidth = FONT_SIZE_X1;
	pfs.charHeight = FONT_SIZE_X1;
	pfs.justification = FONT_JUSTIFICATION_LEFT;
	pfs.bLineSpacing = 100;
	pfs.emphasized = TRUE;
	pfs.italic = FALSE;
	pfs.leftMarginValue = 50;

	/*
	
	//DoPrintLine(L"test");
	DoPrintLine(L"Alter: 1", pfs);
	DoPrintLine(L"PLZ:   50825", pfs);
	pfs.bLineSpacing = 60;
	DoPrintLine(L"Ja, aber woanders.", pfs);
	pfs.bLineSpacing = 100;
	DoPrintLine(L"100%   ████████████████████", pfs);
	pfs.bLineSpacing = 60;
	DoPrintLine(L"Naja, mir egal.", pfs);
	pfs.bLineSpacing = 100;
	DoPrintLine(L"75%    ███████████████", pfs);
	pfs.bLineSpacing = 60;
	DoPrintLine(L"Ja, weniger ist mehr.", pfs);
	pfs.bLineSpacing = 100;
	DoPrintLine(L"12%    ██▌", pfs);
	pfs.bLineSpacing = 60;
	DoPrintLine(L"Ja, aber Veränderung nein.", pfs);
	pfs.bLineSpacing = 100;
	DoPrintLine(L"45%    █████████", pfs);
	
	DoCut();
	*/
	openFile();

	while (true)
	{

	}
	
	// DeInit library
	
	deInitPrinter();

	return EXIT_SUCCESS;
}