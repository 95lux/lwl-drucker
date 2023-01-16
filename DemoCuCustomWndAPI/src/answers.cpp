#include "stdafx.h"
#include "../include/answers.h"
#include <cmath>
#include <ctgmath>
#include <stdio.h>
#include <iostream>

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
	int total_typ[] = {0,0,0,0};

	for (int i = 0; i < ANZ_FRAGEN; i++) {
		// Gesamt aller Typzuweisungen zählen
		for (int j = 0; j < 4; j++) {
			if (answers_arr[i] != 0) {
				if (answers_mask[i][j] == 1) {
					total_typ[0]++;
				}
				if (answers_mask[i][j] == 2) {
					total_typ[1]++;
				}
				if (answers_mask[i][j] == 3) {
					total_typ[2]++;
				}
				if (answers_mask[i][j] == 4) {
					total_typ[3]++;
				}
			}
		}
		// Fragen Auswerten
		int answer = answers_arr[i];
		if (answer != 0)
			percent_typ[answers_mask[i][answer - 1] - 1] ++;
	}

	// distribute rounding loss based on highest decimal part. 
	double double_percent_typ[4];

	for (int i = 0; i < 4; i++) {
		double_percent_typ[i] = (percent_typ[i] * 100) / total_typ[i];
		percent_typ[i] = (percent_typ[i] * 100) / total_typ[i];
		double_percent_typ[i] -= percent_typ[i];
	}
	
	int loss = 100 - (percent_typ[0] + percent_typ[1] + percent_typ[2] + percent_typ[3]);

	for (int i = 0; i < loss; i++) {
		int max_index = 0;
		for (int j = 0; j < 4; j++) {
			if (double_percent_typ[j] > double_percent_typ[max_index] && double_percent_typ[j] != 0.0) {
				max_index = j;
				// reset already used remainder
				double_percent_typ[j] = 0;
			}
			// in case only .000000 decimal numbers are calculated and a round error still exists
			else if(percent_typ[j] != 0) {
				max_index = j;
			}
		}

		// add rounding loss according to highest double remainder
		percent_typ[max_index] += 1;
	}
	// print percentages to console
	std::cout << "[i] Percentages: ";
	for (int i = 0; i < 4;i++) {
		std::cout << percent_typ[i] << "% ";
	}
	std:: cout << std::endl;
	return;
}

// return a bar as string of unicode chars
wchar_t* answers::get_bar(int typ) {
	wchar_t ret[20] = L"";
	double qnt_chars = percent_typ[typ] / 100.0 * 20;
	int for_loop_cap = (int)round(qnt_chars + 0.5);
	if ((int)qnt_chars == 20) {
		return L"████████████████████";
	}
	else {
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