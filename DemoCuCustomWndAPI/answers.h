#pragma once
class answers {

public:
	unsigned short alter;
	unsigned short plz;
	unsigned short answers_arr[8];

	double percent_typ[4] = {0,0,0,0};

	// enth�lt die Maske f�r die Typzuweisung pro Frage
	// X = fragenID, Y = Antwort (a,b,c,d), Wert = Typzuweisung (int 1-4)
	// Falls mehrere Typzuweisungen pro Frage gew�hlt werden sollen, gibt es ein dreidimensionales Array
	int answers_mask[8][4]; 
	
	// constructor
	answers(int alter, int plz, int answers_arr[], int answers_mask[8][4]);
	
	void calcPercentage();

	char* get_bar(int typ);
};

