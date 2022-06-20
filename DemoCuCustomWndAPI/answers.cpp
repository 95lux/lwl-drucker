#include "stdafx.h"
#include "answers.h"

answers::answers(int alter, int plz, int a1, int a2, int a3, 
	int a4, int a5, int a6, int a7, int a8){

}

void answers::calcPercentage() {
	int total_typ1 = 0;
	int total_typ2 = 0;
	int total_typ3 = 0;
	int total_typ4 = 0;

	for (int i = 0; i < 8; i++) {
		// Gesamt aller Typzuweisungen zählen
		for (int j = 0; j < 4; j++) {
			if (answers_mask[i][j] == 1) {
				total_typ1++;
			}
			if (answers_mask[i][j] == 2) {
				total_typ2++;
			}
			if (answers_mask[i][j] == 3) {
				total_typ3++;
			}
			if (answers_mask[i][j] == 4) {
				total_typ4++;
			}
		}
		// Fragen Auswerten
		int answer = answers_arr[i];
		if (answer == answers_mask[i][0]) {
			percent_typ1++;
		}
		if (answer == answers_mask[i][1]) {
			percent_typ2++;
		}
		if (answer == answers_mask[i][2]) {
			percent_typ3++;
		}
		if (answer == answers_mask[i][3]) {
			percent_typ4++;
		}
	}
	percent_typ1 = percent_typ1 / total_typ1;
	percent_typ2 = percent_typ2 / total_typ2;
	percent_typ3 = percent_typ3 / total_typ3;
	percent_typ4 = percent_typ4 / total_typ4;
	
	return;
}