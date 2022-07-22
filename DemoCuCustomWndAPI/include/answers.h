#pragma once
#define ANZ_FRAGEN 9

#include "drucker.h"
#include <string>

class answers {

public:
	unsigned short alter;
	unsigned short plz;
	unsigned short answers_arr[ANZ_FRAGEN];

	char line[64];

	int percent_typ[4] = {0,0,0,0};

	// enth�lt die Maske f�r die Typzuweisung pro Frage
	// X = fragenID, Y = Antwort (a,b,c,d), Wert = Typzuweisung (int 1-4)
	// Falls mehrere Typzuweisungen pro Frage gew�hlt werden sollen, gibt es ein dreidimensionales Array
	int answers_mask[ANZ_FRAGEN][4];
	
	// constructors
	answers(int , int , int [], int [ANZ_FRAGEN][4]);
	answers();
	
	void calcPercentage();

	wchar_t* get_bar(int);

	void print_question(int , PrintFontStruct);
};

