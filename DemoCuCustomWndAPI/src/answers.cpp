#include "stdafx.h"
#include "../include/answers.h"
#include <cmath>
#include <ctgmath>
#include "../include/drucker.h"

#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)

answers::answers(){};

answers::answers(int alter, int plz, int answers_arr[], int answers_mask[9][4]) : alter(alter), plz(plz) {
	char buf[32] = "";
	sprintf(buf, "%d", alter);
	strcpy(line, buf);
	strcat(line, ",");

	sprintf(buf, "%d", plz);
	strcat(line, buf);
	for (int i = 0; i < ANZ_FRAGEN; i++) {
		this->answers_arr[i] = answers_arr[i];
		strcat(line, ",");
		sprintf(buf, "%d", answers_arr[i]);
		strcat(line, buf);

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
			if (answers_arr[i] != 0) {
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
	wchar_t ret[20] = L"";
	double qnt_chars = percent_typ[typ] / 100.0 * 20;
	int for_loop_cap = (int)round(qnt_chars + 0.5);
	for (int i = 0; i < for_loop_cap; i++) {
		if (i + 1 == for_loop_cap) { // last for loop iteration
			// append half bar if needed
			if (round(qnt_chars) - qnt_chars > 0) {
				// strcat(ret, "▌");
				wcscat(ret, L"▌");
			}
		}
		else { // all loops instead of last one
			wcscat(ret, L"█");
			// strcat(ret, "█");
		}
	}
	// wchar_t* real_ret = (wchar_t*)malloc(sizeof(char) * 20);;
	// wcscpy(real_ret, ret);
	return ret;
}

void answers::print_question(int num, PrintFontStruct pfs) {
	wchar_t buf_int[30];
	wchar_t buf_str[30];
	swprintf(buf_int, sizeof buf_int, L"%d", percent_typ[num]);
	wcscpy(buf_str, L"%  ");
	wcscat(buf_int, buf_str);
	wcscat(buf_int, get_bar(num));
	DoPrintLine(buf_int, pfs);
}