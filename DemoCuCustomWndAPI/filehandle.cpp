#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>


using namespace std;

fstream newfile;

void openFile() {
    newfile.open("tpoint.txt", ios::out);  // open a file to perform write operation using file object
    if (newfile.is_open()) //checking whether the file is open
    {
        newfile << "Tutorials point \n";   //inserting text
        newfile.close();    //close the file object
    }
    newfile.open("inputs.txt", ios::in); //open a file to perform read operation using file object
    if (newfile.is_open()) {   //checking whether the file is open
        string tp;
        while (getline(newfile, tp)) { //read data from file object and put it into string.
            cout << tp << "\n"; //print the data of the string
        }
        newfile.close(); //close the file object.
    }
}