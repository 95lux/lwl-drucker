#pragma once
class answers {
	unsigned short alter;
	unsigned short plz;
	unsigned short answers_arr[8];

	int percent_typ1;
	int percent_typ2;
	int percent_typ3;
	int percent_typ4;

	// enthält die Maske für die Typzuweisung pro Frage
	// X = fragenID, Y = Antwort, Wert = Typzuweisung (int)
	// Falls mehrere Typzuweisungen pro Frage gewählt werden sollen, gibt es ein dreidimensionales Array
	int answers_mask[8][4]; 
	
	// constructor
	answers(int alter, int plz, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);
	
	void calcPercentage();
};

