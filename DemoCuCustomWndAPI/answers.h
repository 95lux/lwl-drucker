#pragma once
#define ANZ_FRAGEN 9

class answers {

public:
	unsigned short alter;
	unsigned short plz;
	unsigned short answers_arr[ANZ_FRAGEN];

	int percent_typ[4] = {0,0,0,0};

	// enthält die Maske für die Typzuweisung pro Frage
	// X = fragenID, Y = Antwort (a,b,c,d), Wert = Typzuweisung (int 1-4)
	// Falls mehrere Typzuweisungen pro Frage gewählt werden sollen, gibt es ein dreidimensionales Array
	int answers_mask[ANZ_FRAGEN][4];
	
	// constructor
	answers(int , int , int [], int [ANZ_FRAGEN][4]);
	
	void calcPercentage();

	wchar_t* get_bar(int typ);
};

