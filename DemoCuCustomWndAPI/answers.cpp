#include "stdafx.h"
#include "answers.h"
#include <cmath>
#include <ctgmath>

#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)

answers::answers(int alter, int plz, int answers_arr[], int answers_mask[8][4]) {
	for (int i = 0; i < 8; i++) {
		this->answers_arr[i] = answers_arr[i];
		for (int j = 0; j < 4; j++) {
			this->answers_mask[i][j] = answers_mask[i][j];
		}
	}
	calcPercentage();
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
			percent_typ[0]++;
		}
		if (answer == answers_mask[i][1]) {
			percent_typ[1]++;
		}
		if (answer == answers_mask[i][2]) {
			percent_typ[2]++;
		}
		if (answer == answers_mask[i][3]) {
			percent_typ[3]++;
		}
	}
	percent_typ[0] = (double)percent_typ[0] / total_typ1;
	percent_typ[1] = (double)percent_typ[1] / total_typ2;
	percent_typ[2] = (double)percent_typ[2] / total_typ3;
	percent_typ[3] = (double)percent_typ[3] / total_typ4;
	
	return;
}

// return a bar as string of unicode chars
char* answers::get_bar(int typ) {
	char ret[20] = "";
	double qnt_chars = percent_typ[typ] * 20;
	int for_loop_cap = (int)round(qnt_chars + 0.5);
	for (int i = 0; i < for_loop_cap; i++) {
		if (i + 1 == for_loop_cap) { // last for loop iteration
			if (round(qnt_chars) - qnt_chars < 0) {
				// strcat(ret, "▌");
				strcat(ret, "a");
			}
		}
		else { // all loops instead of last one
			strcat(ret, "b");
			// strcat(ret, "█");
		}
	}
	return ret;
}