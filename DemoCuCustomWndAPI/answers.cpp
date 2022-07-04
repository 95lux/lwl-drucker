#include "stdafx.h"
#include "answers.h"
#include <cmath>
#include <ctgmath>

#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)



answers::answers(int alter, int plz, int answers_arr[], int answers_mask[9][4]) : alter(alter), plz(plz) {
	for (int i = 0; i < ANZ_FRAGEN; i++) {
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

	for (int i = 0; i < ANZ_FRAGEN; i++) {
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
	percent_typ[0] = (percent_typ[0] * 100) / total_typ1;
	percent_typ[1] = (percent_typ[1] * 100) / total_typ2;
	percent_typ[2] = (percent_typ[2] * 100) / total_typ3;
	percent_typ[3] = (percent_typ[3] * 100) / total_typ4;
	
	return;
}

// return a bar as string of unicode chars
wchar_t* answers::get_bar(int typ) {
	char ret[20] = "";
	double qnt_chars = percent_typ[typ] / 100.0 * 20;
	int for_loop_cap = (int)round(qnt_chars + 0.5);
	for (int i = 0; i < for_loop_cap; i++) {
		if (i + 1 == for_loop_cap) { // last for loop iteration
			// append half bar if needed
			if (round(qnt_chars) - qnt_chars > 0) {
				// strcat(ret, "▌");
				strcat(ret, "▌");
			}
		}
		else { // all loops instead of last one
			strcat(ret, "█");
			// strcat(ret, "█");
		}
	}
	wchar_t* real_ret = (wchar_t*)malloc(sizeof(char) * 20);;
	wcscpy(real_ret, (wchar_t*)ret);
	return real_ret;
}
