#include "stdafx.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>  
#include <time.h>

#include   "..\include\ccw_types.h"
#include   "..\include\ccw_DeviceImpl.h"

int initPrinter(const wchar_t* printer_com);

void deInitPrinter();

static int LoadProcs(HMODULE   Dll);

void DoDeInitLibrary();

void DoGetLibRel();

void DoCloseDevice();

int DoSelectCOMDevice(COMPortStruct * comportsList, DWORD dwDeviceNum);

void DoOpenCOMPrinter(const wchar_t* printer_com);

int DoSelectPrinter(PrinterStruct * printersList, DWORD dwPrintersNum);

void DoOpenPrinter();

void DoPrintLine(wchar_t* Line, PrintFontStruct pfs);

void DoPrintBarcode();

void DoPrintImage(const wchar_t *);

void DoCut();

void DoEject();

void DoFeed();

void DoGetStatus();