#include "stdafx.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>  
#include <time.h>

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
	// Now, de-init library
	CcwResult Result = FnDeInitLibrary();

	printf("DeInitalize: Result = %d (%ws)\n", Result, CcwResultToText(Result));

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

int DoSelectUSBDevice(USBDeviceStruct* usbdevicesList, DWORD dwDeviceNum)
{
	char           Line[32] = { 0 };

	printf("\nUSB Device list:(0 to exit)\n");
	// For every connected device
	for (DWORD dw = 0; dw < dwDeviceNum; dw++)
	{
		printf(" [Index: %d, VID: 0x%04X | PID: 0x%04X | SN: 0x%ws | Type: %d | Printer Name: <%ws>] \n",

			(dw + 1),
			usbdevicesList[dw].wVID,
			usbdevicesList[dw].wPID,
			usbdevicesList[dw].cSerialNumber,
			usbdevicesList[dw].uIntType,
			usbdevicesList[dw].cPrinterName

		);

	}

	do
	{
		printf("\nEnter device INDEX: ");
		if (fgets(Line, sizeof(Line), stdin) != NULL)
		{
			DWORD n = strtoul(Line, NULL, 0);
			if (n == 0)
			{
				// Index not valid
				printf("Index out of range.\n");
				return -1;
			}

			if ((n < (dwDeviceNum + 1)))
			{
				printf("Selected device with index: %d\n", n);
				return (n - 1);
			}
			else
			{
				// Index not valid
				printf("Index out of range.\n");
				return -1;
			}

		}
	} while (1);

	return -1;
}

void DoOpenUSBPrinter()
{
	CcwResult Result;
	DWORD dwListElementsNumber;
	USBDeviceStruct* pUSBDeviceStructArray;

	//Extract the list of the USB Devices
	Result = FnEnumUSBDevices(NULL, &dwListElementsNumber);
	if (Result != CcwResult_OK)
	{
		//Error
		printf("DoOpenUSBPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	//If no USB Devices, exit
	if (dwListElementsNumber == 0)
	{
		//Error
		printf("DoOpenUSBPrinter: No USB devices found!\n");
		return;
	}

	//Create the array and get again the enum
	pUSBDeviceStructArray = new USBDeviceStruct[dwListElementsNumber];
	Result = FnEnumUSBDevices(pUSBDeviceStructArray, &dwListElementsNumber);
	if (Result != CcwResult_OK)
	{
		// Free memory
		delete[] pUSBDeviceStructArray;

		//Error
		printf("DoOpenUSBPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	// Show a list of devices an ask the user to select one 
	int indexdevice = DoSelectUSBDevice(pUSBDeviceStructArray, dwListElementsNumber);
	if (indexdevice < 0)
	{
		// Free memory
		delete[] pUSBDeviceStructArray;

		//Error
		printf("DoOpenUSBPrinter: No device select\n");
		return;
	}

	//Open device
	Result = FnOpenPrinterUSBEx(&pUSBDeviceStructArray[indexdevice], &m_dwDeviceID);
	// Free memory
	delete[] pUSBDeviceStructArray;
	if (Result != CcwResult_OK)
	{
		//Error
		printf("DoOpenUSBPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	m_bDeviceOpen = TRUE;

	printf("DoOpenUSBPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
}

int DoSelectETHDevice(ETHDeviceStruct* ethdevicesList, DWORD dwDeviceNum)
{
	char           Line[32] = { 0 };

	printf("\nETH Device list:(0 to exit)\n");
	// For every connected device
	for (DWORD dw = 0; dw < dwDeviceNum; dw++)
	{
		printf(" [Index: %d, NAME: %ws | IP: %ws] \n",

			(dw + 1),
			ethdevicesList[dw].cDeviceName,
			ethdevicesList[dw].cIPAddress

		);

	}

	do
	{
		printf("\nEnter device INDEX: ");
		if (fgets(Line, sizeof(Line), stdin) != NULL)
		{
			DWORD n = strtoul(Line, NULL, 0);
			if (n == 0)
			{
				// Index not valid
				printf("Index out of range.\n");
				return -1;
			}

			if ((n < (dwDeviceNum + 1)))
			{
				printf("Selected device with index: %d\n", n);
				return (n - 1);
			}
			else
			{
				// Index not valid
				printf("Index out of range.\n");
				return -1;
			}

		}
	} while (1);

	return -1;
}

void DoOpenETHPrinter()
{
	CcwResult Result;
	DWORD dwListElementsNumber;
	ETHDeviceStruct* pETHDeviceStructArray;

	//Extract the list of the ETH Devices
	Result = FnEnumETHDevices(NULL, &dwListElementsNumber, 500);
	if (Result != CcwResult_OK)
	{
		//Error
		printf("DoOpenETHPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	//If no ETH Devices, exit
	if (dwListElementsNumber == 0)
	{
		//Error
		printf("DoOpenETHPrinter: No ETH devices found!\n");
		return;
	}

	//Create the array and get again the enum
	pETHDeviceStructArray = new ETHDeviceStruct[dwListElementsNumber];
	Result = FnEnumETHDevices(pETHDeviceStructArray, &dwListElementsNumber, 500);
	if (Result != CcwResult_OK)
	{
		// Free memory
		delete[] pETHDeviceStructArray;

		//Error
		printf("DoOpenETHPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	// Show a list of devices an ask the user to select one 
	int indexdevice = DoSelectETHDevice(pETHDeviceStructArray, dwListElementsNumber);
	if (indexdevice < 0)
	{
		// Free memory
		delete[] pETHDeviceStructArray;

		//Error
		printf("DoOpenETHPrinter: No device select\n");
		return;
	}

	//Open device
	Result = FnOpenPrinterETHEx(&pETHDeviceStructArray[indexdevice], &m_dwDeviceID);
	// Free memory
	delete[] pETHDeviceStructArray;
	if (Result != CcwResult_OK)
	{
		//Error
		printf("DoOpenETHPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	m_bDeviceOpen = TRUE;

	printf("DoOpenETHPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
}

int DoSelectCOMDevice(COMPortStruct* comportsList, DWORD dwDeviceNum)
{
	char Line[32] = { 0 };
	DWORD n = 1;
	printf("Selected device with index: %d\n", n);
	return n;
}

void DoOpenCOMPrinter()
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

	// Show a list of devices an ask the user to select one 
	int indexdevice = 1;
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

int DoSelectPrinter(PrinterStruct* printersList, DWORD dwPrintersNum)
{
	char           Line[32] = { 0 };

	printf("\nPrinters list:\n");
	// For every printer
	for (DWORD dw = 0; dw < dwPrintersNum; dw++)
	{
		printf(" [Index: %d, Printer Name: <%ws> Online:%d Default:%d] \n",

			(dw + 1),

			printersList[dw].cPrinterName,
			printersList[dw].bPrinterOnline,
			printersList[dw].bDefaultPrinter

		);

	}

	do
	{
		printf("\nEnter printer INDEX: ");
		if (fgets(Line, sizeof(Line), stdin) != NULL)
		{
			DWORD n = strtoul(Line, NULL, 0);
			if (n == 0)
			{
				// Index not valid
				printf("Index out of range.\n");
				return -1;
			}

			if ((n < (dwPrintersNum + 1)))
			{
				printf("Selected printer with index: %d\n", n);
				return (n - 1);
			}
			else
			{
				// Index not valid
				printf("Index out of range.\n");
				return -1;
			}

		}
	} while (1);

	return -1;
}

void DoOpenPrinter()
{
	CcwResult Result;
	DWORD dwListElementsNumber;
	PrinterStruct* pPrinterStructArray;

	//Extract the list of the Printers
	Result = FnEnumInstalledPrinters(NULL, &dwListElementsNumber);
	if (Result != CcwResult_OK)
	{
		//Error
		printf("DoOpenPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	//If no Printers, exit
	if (dwListElementsNumber == 0)
	{
		//Error
		printf("DoOpenPrinter: No printers found!\n");
		return;
	}

	//Create the array and get again the enum
	pPrinterStructArray = new PrinterStruct[dwListElementsNumber];
	Result = FnEnumInstalledPrinters(pPrinterStructArray, &dwListElementsNumber);
	if (Result != CcwResult_OK)
	{
		// Free memory
		delete[] pPrinterStructArray;

		//Error
		printf("DoOpenPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	// Show a list of devices an ask the user to select one 
	int indexdevice = DoSelectPrinter(pPrinterStructArray, dwListElementsNumber);
	if (indexdevice < 0)
	{
		// Free memory
		delete[] pPrinterStructArray;

		//Error
		printf("DoOpenPrinter: No printer select\n");
		return;
	}

	//Open device
	Result = FnOpenInstalledPrinter(pPrinterStructArray[indexdevice].cPrinterName, &m_dwDeviceID);
	// Free memory
	delete[] pPrinterStructArray;
	if (Result != CcwResult_OK)
	{
		//Error
		printf("DoOpenPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
		return;
	}

	//Print some device info: Printer model
	wchar_t devModel[MAXCHARBUFFER] = { 0 };
	Result = FnGetInfoDeviceModel(m_dwDeviceID, devModel);
	printf("Printer Model: (%ws)\n", devModel);
	//Print some device info: Firmware version
	wchar_t devFWVersion[MAXCHARBUFFER] = { 0 };
	Result = FnGetInfoFirmwareVersion(m_dwDeviceID, devFWVersion);
	printf("Firmware Version: (%ws)\n", devFWVersion);

	m_bDeviceOpen = TRUE;

	printf("DoOpenPrinter: Result = %d (%ws)\n", Result, CcwResultToText(Result));
}

void DoPrintLine(wchar_t *Line)
{
	// wchar_t   Line[MAXCHARBUFFER] = { 0 };
	// wchar_t   Line[MAXCHARBUFFER] = "Test";
	CcwResult Result;

	//Function available ONLY if a device is open
	if (!m_bDeviceOpen)
	{
		printf("Close: Error - no device selected\n");
		return;
	}

	//Create a sample font attributes / properties
	PrintFontStruct pfs = PRINTFONTSTRUCT_INIT;
	pfs.charWidth = FONT_SIZE_X1;
	pfs.charHeight = FONT_SIZE_X2;
	pfs.justification = FONT_JUSTIFICATION_CENTER;
	pfs.emphasized = TRUE;

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
	wchar_t   Line[MAXCHARBUFFER] = { 0 };
	CcwResult Result;

	//Function available ONLY if a device is open
	if (!m_bDeviceOpen)
	{
		printf("Close: Error - no device selected\n");
		return;
	}

	//Ask the text
	do
	{
		printf("\nEnter the text to print into barcode: ");
		if (fgetws(Line, sizeof(Line), stdin) != NULL)
		{
			if (wcslen(Line) > 1)
				break;
		}
	} while (1);

	//Remove the LF at the end of the string
	Line[wcslen(Line) - 1] = 0x00;
	printf("Printing <%ws> barcode...\n", Line);

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

void DoPrintImage()
{
	wchar_t   Line[MAXCHARBUFFER] = { 0 };
	CcwResult Result;

	//Function available ONLY if a device is open
	if (!m_bDeviceOpen)
	{
		printf("Close: Error - no device selected\n");
		return;
	}

	//Ask the text
	do
	{
		printf("\nEnter the path of the image to print: ");
		if (fgetws(Line, sizeof(Line), stdin) != NULL)
		{
			if (wcslen(Line) > 1)
				break;
		}
	} while (1);

	printf("Printing <%ws>...\n", Line);
	//Remove the LF at the end of the string
	Line[wcslen(Line) - 1] = 0x00;

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

int _tmain(int argc, _TCHAR* argv[])
{

	char* linebuffer[32];

	int			Result;
	HMODULE		cDll;
	wchar_t* DllName = L"CuCustomWndAPI.dll";
	CcwResult	ret;
	CcwLogVerbosity	logVerbosity = CCW_LOG_WARNING;			// Deafult value

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
	DoOpenCOMPrinter();

	while (true)
	{
		DoPrintLine(L"test");
	}

	// DeInit library
	DoDeInitLibrary();

	FreeLibrary(cDll);

	printf("Library '%S' closed\n", DllName);
	return EXIT_SUCCESS;
}
