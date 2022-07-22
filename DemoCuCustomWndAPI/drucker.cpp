#include "stdafx.h"
#include "drucker.h"
#include "conio.h"

#include   "..\include\ccw_types.h"
#include   "..\include\ccw_DeviceImpl.h"

#define   LoadAndCheck(Func, Type, Handle, Name, Result)                                          \
	if ((Func = (Type) GetProcAddress (Handle, Name)) == NULL)          \
							  {                                                                   \
							  Result++;                                                       \
							  fprintf (stderr, "Entry point '%s' not found in DLL\n", Name);  \
							  }

INITLIBRARY_FN					FnInitLibrary = NULL;
DEINITLIBRARY_FN				FnDeInitLibrary = NULL;
CLOSEDEVICE_FN					FnCloseDevice = NULL;
CLOSEALLDEVICES_FN				FnCloseAllDevices = NULL;

GETAPIVERSION_FN				FnGetAPIVersion = NULL;

GETINFOPRINTERID_FN				FnGetInfoPrinterId = NULL;
GETINFOFIRMWAREVERSION_FN		FnGetInfoFirmwareVersion = NULL;
GETINFODEVICEMODEL_FN			FnGetInfoDeviceModel = NULL;

GETCAPCOMMPORTTYPE_FN			FnGetCapCommPortType = NULL;
GETCAPPRINTAVAILABLE_FN			FnGetCapPrintAvailable = NULL;
GETCAPPRINTERRESOLUTION_FN		FnGetCapPrinterResolution = NULL;
GETCAPPRINTWIDTH_FN				FnGetCapPrintWidth = NULL;
SETCAPPRINTWIDTH_FN				FnSetCapPrintWidth = NULL;

COMMGETREADTIMEOUT_FN			FnCommGetReadTimeout = NULL;
COMMSETREADTIMEOUT_FN			FnCommSetReadTimeout = NULL;
COMMGETWRITETIMEOUT_FN			FnCommGetWriteTimeout = NULL;
COMMSETWRITETIMEOUT_FN			FnCommSetWriteTimeout = NULL;
COMMCLEARREADBUFFER_FN			FnCommClearReadBuffer = NULL;
COMMCLEARWRITEBUFFER_FN			FnCommClearWriteBuffer = NULL;
COMMREAD_FN						FnCommRead = NULL;
COMMWRITE_FN					FnCommWrite = NULL;

FEED_FN							FnFeed = NULL;
CUT_FN							FnCut = NULL;
EJECT_FN						FnEject = NULL;
PRESENT_FN						FnPresent = NULL;
ALIGN_FN						FnAlign = NULL;

PRINTERISREADY_FN				FnPrinterIsReady = NULL;
GETPRINTERFULLSTATUS_FN			FnGetPrinterFullStatus = NULL;

PRINTTEXT_FN					FnPrintText = NULL;
PRINTTEXTEX_FN					FnPrintTextEx = NULL;

SETIGNOREWHITEIMAGES_FN			FnSetIgnoreWhiteImages = NULL;
SETGRMODETOBLACKWHITE_FN		FnSetGrModeToBlackWhite = NULL;
SETPAGEMODETOROLL_FN			FnSetPageModeToRoll = NULL;
PRINTIMAGEFROMPATH_FN			FnPrintImageFromPath = NULL;
PRINTRAWIMAGE_FN				FnPrintRawImage = NULL;

PRINTBARCODESTRING_FN			FnPrintBarcodeString = NULL;
PRINTBARCODEBUFFER_FN			FnPrintBarcodeBuffer = NULL;

GETVARIABLE_FN					FnGetVariable = NULL;
SETVARIABLE_FN					FnSetVariable = NULL;

RAWDATATOPRINTSPOOLER_FN		FnRawDataToPrintSpooler = NULL;
BINFILETOPRINTSPOOLER_FN		FnBinFileToPrintSpooler = NULL;
ENUMINSTALLEDPRINTERS_FN		FnEnumInstalledPrinters = NULL;
OPENINSTALLEDPRINTER_FN			FnOpenInstalledPrinter = NULL;

ENUMUSBDEVICES_FN				FnEnumUSBDevices = NULL;
OPENPRINTERUSB_FN				FnOpenPrinterUSB = NULL;
OPENPRINTERUSBEX_FN				FnOpenPrinterUSBEx = NULL;

ENUMETHDEVICES_FN				FnEnumETHDevices = NULL;
OPENPRINTERETH_FN				FnOpenPrinterETH = NULL;
OPENPRINTERETHEX_FN				FnOpenPrinterETHEx = NULL;

ENUMCOMPORTS_FN					FnEnumCOMPorts = NULL;
OPENPRINTERCOM_FN				FnOpenPrinterCOM = NULL;
OPENPRINTERCOMEX_FN				FnOpenPrinterCOMEx = NULL;

BOOL							m_bDeviceOpen = FALSE;
DWORD							m_dwDeviceID = 0;

int			Result;
HMODULE		cDll;
wchar_t* DllName = L"CuCustomWndAPI.dll";
CcwResult	ret;
CcwLogVerbosity	logVerbosity = CCW_LOG_WARNING;			// Default value

int initPrinter(char* printer_com) {

	// Load library	
	cDll = LoadLibraryW(DllName);
	if (cDll == NULL)
	{
		fprintf(stderr, "Error: Unable to load library '%S'\n", DllName);
		fprintf(stderr, "Press any key to close\n");
		getchar();

		// Error!
		return EXIT_FAILURE;
	}
	printf("Library %S successfully loaded.\n", DllName);

	//Load functions
	if ((Result = LoadProcs(cDll)) != 0)
	{
		fprintf(stderr, "Error: %d function(s) not found in dll '%S'\n", Result, DllName);
		getchar();
		return EXIT_FAILURE;
	}

	printf("Functions successfully loaded.\n");

	// Initialize library
	ret = FnInitLibrary(logVerbosity, NULL);
	if (ret != CcwResult_OK)
	{
		// Initialization error. Exit
		getchar();

		FreeLibrary(cDll);

		// Error!
		return EXIT_FAILURE;
	}

	// Get library release
	DoGetLibRel();

	// Open COM Device with Index 1
	DoOpenCOMPrinter(printer_com);
};

void deInitPrinter() {
	FnDeInitLibrary();

	FreeLibrary(cDll);

	printf("Library '%S' closed\n", DllName);
}

static int LoadProcs(HMODULE   Dll)
{
	int   Result = 0;

	LoadAndCheck(FnInitLibrary, INITLIBRARY_FN, Dll, "InitLibrary", Result);
	LoadAndCheck(FnDeInitLibrary, DEINITLIBRARY_FN, Dll, "DeInitLibrary", Result);
	LoadAndCheck(FnCloseDevice, CLOSEDEVICE_FN, Dll, "CloseDevice", Result);
	LoadAndCheck(FnCloseAllDevices, CLOSEALLDEVICES_FN, Dll, "CloseAllDevices", Result);

	LoadAndCheck(FnGetAPIVersion, GETAPIVERSION_FN, Dll, "GetAPIVersion", Result);

	LoadAndCheck(FnGetInfoPrinterId, GETINFOPRINTERID_FN, Dll, "GetInfoPrinterId", Result);
	LoadAndCheck(FnGetInfoFirmwareVersion, GETINFOFIRMWAREVERSION_FN, Dll, "GetInfoFirmwareVersion", Result);
	LoadAndCheck(FnGetInfoDeviceModel, GETINFODEVICEMODEL_FN, Dll, "GetInfoDeviceModel", Result);

	LoadAndCheck(FnGetCapCommPortType, GETCAPCOMMPORTTYPE_FN, Dll, "GetCapCommPortType", Result);
	LoadAndCheck(FnGetCapPrintAvailable, GETCAPPRINTAVAILABLE_FN, Dll, "GetCapPrintAvailable", Result);
	LoadAndCheck(FnGetCapPrinterResolution, GETCAPPRINTERRESOLUTION_FN, Dll, "GetCapPrinterResolution", Result);
	LoadAndCheck(FnGetCapPrintWidth, GETCAPPRINTWIDTH_FN, Dll, "GetCapPrintWidth", Result);
	LoadAndCheck(FnSetCapPrintWidth, SETCAPPRINTWIDTH_FN, Dll, "SetCapPrintWidth", Result);

	LoadAndCheck(FnCommGetReadTimeout, COMMGETREADTIMEOUT_FN, Dll, "CommGetReadTimeout", Result);
	LoadAndCheck(FnCommSetReadTimeout, COMMSETREADTIMEOUT_FN, Dll, "CommSetReadTimeout", Result);
	LoadAndCheck(FnCommGetWriteTimeout, COMMGETWRITETIMEOUT_FN, Dll, "CommGetWriteTimeout", Result);
	LoadAndCheck(FnCommSetWriteTimeout, COMMSETWRITETIMEOUT_FN, Dll, "CommSetWriteTimeout", Result);
	LoadAndCheck(FnCommClearReadBuffer, COMMCLEARREADBUFFER_FN, Dll, "CommClearReadBuffer", Result);
	LoadAndCheck(FnCommClearWriteBuffer, COMMCLEARWRITEBUFFER_FN, Dll, "CommClearWriteBuffer", Result);
	LoadAndCheck(FnCommRead, COMMREAD_FN, Dll, "CommRead", Result);
	LoadAndCheck(FnCommWrite, COMMWRITE_FN, Dll, "CommWrite", Result);

	LoadAndCheck(FnFeed, FEED_FN, Dll, "Feed", Result);
	LoadAndCheck(FnCut, CUT_FN, Dll, "Cut", Result);
	LoadAndCheck(FnEject, EJECT_FN, Dll, "Eject", Result);
	LoadAndCheck(FnPresent, PRESENT_FN, Dll, "Present", Result);
	LoadAndCheck(FnAlign, ALIGN_FN, Dll, "Align", Result);

	LoadAndCheck(FnPrinterIsReady, PRINTERISREADY_FN, Dll, "PrinterIsReady", Result);
	LoadAndCheck(FnGetPrinterFullStatus, GETPRINTERFULLSTATUS_FN, Dll, "GetPrinterFullStatus", Result);

	LoadAndCheck(FnPrintText, PRINTTEXT_FN, Dll, "PrintText", Result);
	LoadAndCheck(FnPrintTextEx, PRINTTEXTEX_FN, Dll, "PrintTextEx", Result);

	LoadAndCheck(FnSetIgnoreWhiteImages, SETIGNOREWHITEIMAGES_FN, Dll, "SetIgnoreWhiteImages", Result);
	LoadAndCheck(FnSetGrModeToBlackWhite, SETGRMODETOBLACKWHITE_FN, Dll, "SetGrModeToBlackWhite", Result);
	LoadAndCheck(FnSetPageModeToRoll, SETPAGEMODETOROLL_FN, Dll, "SetPageModeToRoll", Result);
	LoadAndCheck(FnPrintImageFromPath, PRINTIMAGEFROMPATH_FN, Dll, "PrintImageFromPath", Result);
	LoadAndCheck(FnPrintRawImage, PRINTRAWIMAGE_FN, Dll, "PrintRawImage", Result);

	LoadAndCheck(FnPrintBarcodeString, PRINTBARCODESTRING_FN, Dll, "PrintBarcodeString", Result);
	LoadAndCheck(FnPrintBarcodeBuffer, PRINTBARCODEBUFFER_FN, Dll, "PrintBarcodeBuffer", Result);

	LoadAndCheck(FnGetVariable, GETVARIABLE_FN, Dll, "GetVariable", Result);
	LoadAndCheck(FnSetVariable, SETVARIABLE_FN, Dll, "SetVariable", Result);

	LoadAndCheck(FnRawDataToPrintSpooler, RAWDATATOPRINTSPOOLER_FN, Dll, "RawDataToPrintSpooler", Result);
	LoadAndCheck(FnBinFileToPrintSpooler, BINFILETOPRINTSPOOLER_FN, Dll, "BinFileToPrintSpooler", Result);
	LoadAndCheck(FnEnumInstalledPrinters, ENUMINSTALLEDPRINTERS_FN, Dll, "EnumInstalledPrinters", Result);
	LoadAndCheck(FnOpenInstalledPrinter, OPENINSTALLEDPRINTER_FN, Dll, "OpenInstalledPrinter", Result);

	LoadAndCheck(FnEnumUSBDevices, ENUMUSBDEVICES_FN, Dll, "EnumUSBDevices", Result);
	LoadAndCheck(FnOpenPrinterUSB, OPENPRINTERUSB_FN, Dll, "OpenPrinterUSB", Result);
	LoadAndCheck(FnOpenPrinterUSBEx, OPENPRINTERUSBEX_FN, Dll, "OpenPrinterUSBEx", Result);

	LoadAndCheck(FnEnumETHDevices, ENUMETHDEVICES_FN, Dll, "EnumETHDevices", Result);
	LoadAndCheck(FnOpenPrinterETH, OPENPRINTERETH_FN, Dll, "OpenPrinterETH", Result);
	LoadAndCheck(FnOpenPrinterETHEx, OPENPRINTERETHEX_FN, Dll, "OpenPrinterETHEx", Result);

	LoadAndCheck(FnEnumCOMPorts, ENUMCOMPORTS_FN, Dll, "EnumCOMPorts", Result);
	LoadAndCheck(FnOpenPrinterCOM, OPENPRINTERCOM_FN, Dll, "OpenPrinterCOM", Result);
	LoadAndCheck(FnOpenPrinterCOMEx, OPENPRINTERCOMEX_FN, Dll, "OpenPrinterCOMEx", Result);

	return Result;
}

void DoDeInitLibrary()
{
	FnDeInitLibrary();
	return;
}

void DoGetLibRel()
{
	DWORD dwStrLenInChar = 5;
	wchar_t* szLibRel = new wchar_t[dwStrLenInChar];
	ZeroMemory(szLibRel, dwStrLenInChar * sizeof(wchar_t));

	//Get API version
	CcwResult result = FnGetAPIVersion(szLibRel);
	if (result != CcwResult_OK)
		printf("Error %d (%ws)\n", result, CcwResultToText(result));
	else
		printf("Library release %ws.\n", szLibRel);

	delete[] szLibRel;
}

void DoCloseDevice()
{
	// Already initialized?
	if (!m_bDeviceOpen)
	{
		printf("No device to close!\n");
		return;
	}

	//Close the device
	CcwResult Result = FnCloseDevice(m_dwDeviceID);
	if (Result == CcwResult_OK)
	{
		m_bDeviceOpen = FALSE;
	}

	printf("DeInitalize: Result = %d (%ws)\n", Result, CcwResultToText(Result));
}

int DoSelectCOMDevice(COMPortStruct* comportsList, DWORD dwDeviceNum)
{
	char Line[32] = { 0 };
	DWORD n = 1;
	printf("Selected device with index: %d\n", n);
	return n;
}

void DoOpenCOMPrinter(char* printer_com)
{
	CcwResult Result;
	DWORD dwListElementsNumber;
	COMPortStruct* pCOMPortStructArray;

	//Extract the list of the COM Ports
	Result = FnEnumCOMPorts(NULL, &dwListElementsNumber);
	if (Result != CcwResult_OK)
	{
		//Error
		printf("DoOpenCOMPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	//If no COM ports, exit
	if (dwListElementsNumber == 0)
	{
		//Error
		printf("DoOpenCOMPrinter: No COM ports found!\n");
		return;
	}

	//Create the array and get again the enum
	pCOMPortStructArray = new COMPortStruct[dwListElementsNumber];
	Result = FnEnumCOMPorts(pCOMPortStructArray, &dwListElementsNumber);
	if (Result != CcwResult_OK)
	{
		// Free memory
		delete[] pCOMPortStructArray;

		//Error
		printf("DoOpenCOMPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}
	int indexdevice = 0;
	// Show a list of devices an ask the user to select one 
	for (int i = 0; i < dwListElementsNumber; i++) {
		wchar_t* str = pCOMPortStructArray[i].cCOMPort;
		if (wcscmp(str, (wchar_t*)printer_com) == 0) {
			indexdevice = i;
		}
	}
	if (indexdevice < 0)
	{
		// Free memory
		delete[] pCOMPortStructArray;

		//Error
		printf("DoOpenCOMPrinter: No device select\n");
		return;
	}

	//Open device
	Result = FnOpenPrinterCOMEx(&pCOMPortStructArray[indexdevice], &m_dwDeviceID);
	// Free memory
	delete[] pCOMPortStructArray;
	if (Result != CcwResult_OK)
	{
		//Error
		printf("DoOpenCOMPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	m_bDeviceOpen = TRUE;

	printf("DoOpenCOMPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
}

void DoPrintLine(wchar_t *Line, PrintFontStruct pfs)
{
	CcwResult Result;

	//Function available ONLY if a device is open
	if (!m_bDeviceOpen)
	{
		printf("Close: Error - no device selected\n");
		return;
	}
	//Print Text
	Result = FnPrintTextEx(m_dwDeviceID, Line, &pfs, TRUE);
	if (Result != CcwResult_OK)
	{
		//Error
		printf("DoPrintText: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	printf("DoPrintText: Result = %d (%ws)\n", Result, CcwResultToText(Result));
}

void DoPrintBarcode()
{
	wchar_t   Line[MAXCHARBUFFER] = L"https://power2change-energiewende.de";
	CcwResult Result;

	//Function available ONLY if a device is open
	if (!m_bDeviceOpen)
	{
		printf("Close: Error - no device selected\n");
		return;
	}

	//Create a sample attributes / properties
	PrintBarcodeStruct pfb = PRINTBARCODESTRUCT_INIT;
	pfb.brcAlignMode = BARCODE_ALIGN_TO_CENTER;
	pfb.dwBarcodeWidth = 200;
	pfb.dwBarcodeHeight = 200;
	pfb.brcType = BARCODE_TYPE_QRCODE;

	//Print Barcode
	Result = FnPrintBarcodeString(m_dwDeviceID, Line, &pfb);
	if (Result != CcwResult_OK)
	{
		//Error
		printf("DoPrintBarcode: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	printf("DoPrintBarcode: Result = %d (%ws)\n", Result, CcwResultToText(Result));
}

void DoPrintImage(wchar_t *path)
{
	// wchar_t   Line[MAXCHARBUFFER] = ;
	wchar_t* Line = (wchar_t*)malloc(sizeof(char)*MAXCHARBUFFER);
	Line = path;
	CcwResult Result;

	//Function available ONLY if a device is open
	if (!m_bDeviceOpen)
	{
		printf("Close: Error - no device selected\n");
		return;
	}

	printf("Printing <%ws>...\n", Line);

	//Create a sample image attributes / properties
	PrintImageStruct pis = PRINTIMAGESTRUCT_INIT;
	pis.printScaleMode = IMAGE_SCALE_TO_FIT;

	//Print Image
	Result = FnPrintImageFromPath(m_dwDeviceID, Line, &pis);
	if (Result != CcwResult_OK)
	{
		//Error
		printf("DoPrintImage: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	printf("DoPrintImage: Result = %d (%ws)\n", Result, CcwResultToText(Result));
}

void DoCut()
{
	//Function available ONLY if a device is open
	if (!m_bDeviceOpen)
	{
		printf("Close: Error - no device selected\n");
		return;
	}

	//Cut (total cut)
	CcwResult Result = FnCut(m_dwDeviceID, CUT_TOTAL);
	printf("DoCut: Result = %d (%ws)\n", Result, CcwResultToText(Result));
}

void DoEject()
{
	//Function available ONLY if a device is open
	if (!m_bDeviceOpen)
	{
		printf("Close: Error - no device selected\n");
		return;
	}

	//Eject (eject)
	CcwResult Result = FnEject(m_dwDeviceID, EJ_EJECT);
	printf("DoEject: Result = %d (%ws)\n", Result, CcwResultToText(Result));
}

void DoFeed()
{
	//Function available ONLY if a device is open
	if (!m_bDeviceOpen)
	{
		printf("Close: Error - no device selected\n");
		return;
	}

	//Feed (force to 2)
	CcwResult Result = FnFeed(m_dwDeviceID, 2);
	printf("DoFeed: Result = %d (%ws)\n", Result, CcwResultToText(Result));
}

void DoGetStatus()
{
	//Function available ONLY if a device is open
	if (!m_bDeviceOpen)
	{
		printf("Close: Error - no device selected\n");
		return;
	}

	BYTE bStatus[4];
	PrinterStatusStruct pss;

	//Get Status
	CcwResult Result = FnGetPrinterFullStatus(m_dwDeviceID, bStatus, &pss);
	if (Result != CcwResult_OK)
	{
		//Error
		printf("DoGetStatus: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	//Write the status
	printf("Status: 0x%02X 0x%02X 0x%02X 0x%02X\n", bStatus[0], bStatus[1], bStatus[2], bStatus[3]);
	wchar_t* strText = new wchar_t[MAXCHARBUFFER * 4];
	ZeroMemory(strText, MAXCHARBUFFER * 4 * sizeof(wchar_t));
	PrinterStatusStructToText(&pss, strText, MAXCHARBUFFER * 4);
	printf("%ws\n", strText);
	delete[] strText;

	printf("DoGetStatus: Result = %d (%ws)\n", Result, CcwResultToText(Result));
}


