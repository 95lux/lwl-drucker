OVERVIEW
------------

Printer App written in C++ for [Custom TG2480HIII receipt printer](https://www.custom.biz/en_GB/product/hardware/printers/self-service-kiosk-printers/tg2480hiii)

This program: 
* reads serial data from an M5 RFID reader module via vCOM, 
* evaluates answers saved in the RFID data according to an evaluation matrix, 
* prints out a receipt via given CUSTOM C++ DLL API,
* saves the data as daily .csv logfiles

## Usage

The config.ini file has to be placed in ``C:\235Media\lwldrucker\config.ini``.
In config.ini file following options can be configured:
*  ``logo_path``: Absolute path to logo image file, printed on the receipt.  
    Max resolution: 610x430px  
    File Types: .jpg .bitmap

* ``logfiles_path``: Absolute path to daily .csv logfiles of the poll evaluation

* ``printer_com``: Virtual comport of connected printer 
* ``rfid_com``: Virtual comport of connected M5 RFID reader

If set up correctly a receipt printed and a .csv file with name of the current date will be saved at the configured path.  

## JSON format

Requirement for the app to run correctly is a correctly formatted json string in the first place. M5's JSON over serial uses javascript standard, thus using single quotes for key/value seperation. These will be replace with double quotes by the program.

Following keys are required for the program to run:
* "age"
* "plz"
* "a1"
* "a2"
* "a3"
* "a4"
* "a5"
* "a6"
* "a7"
* "a8"
* "a9"

Example JSON string:  
"{'a4': '0', 'a5': '0', 'a6': '0', 'a7': '0', 'a8': '2', 'a9': '500', 'a1': '1', 'a2': '0', 'a3': '0', 'printed': '1', 'age': '0', 'plz': '4'}";


LIB DEPENDENCIES
------------
* ### jsoncpp
    https://github.com/open-source-parsers/jsoncpp  
    Used to parse incoming serial data (json string)

* ### simpleini
    https://github.com/brofield/simpleini  
    Allows usage of config file in .ini format