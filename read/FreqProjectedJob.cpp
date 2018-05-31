/* This file is part of make_projections.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 laster any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 make_projections 1.0.0 
 Copyright (c) Henrik Quanz
*/


#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include "FreqProjectedJob.h"
#include "../help/HelpFunctions.h"
#ifdef __GNUC__
#include <cstdlib>
#endif

using namespace std;

FreqProjectedJob::FreqProjectedJob(): ZPC(0), thermalCorrectionE(0), thermalCorrectionS(0), thermalCorrectionG(0), sumElZE(0), sumElTE(0), sumElTS(0), sumElTG(0){
}

FreqProjectedJob::FreqProjectedJob(const string& filename):GaussianOut(filename),ZPC(0), thermalCorrectionE(0), thermalCorrectionS(0), thermalCorrectionG(0), sumElZE(0), sumElTE(0), sumElTS(0), sumElTG(0){
    if (!this->GaussianOut::init(filename)) {
        throw fatalGaussianError("An error occured");
    }
    
    if (this->getCalculationType().find("projected") == string::npos) {
        throw fatalGaussianError("You did not submit an projected frequency calculation");
    }
    
    //Read file to string
    fstream inFile(filename.c_str(),ios::in);
    if (!inFile) {
        throw fatalGaussianError("An error occured during opening of file: ");
    }
    
    string contentOfFile;
    help::readFileToString(inFile, contentOfFile);
    
    //Check if all veriables are available
    map<string, size_t> positionsInFile;
    positionsInFile.insert(pair<string, size_t>("ZPC", contentOfFile.find("Zero-point correction=")));
    positionsInFile.insert(pair<string, size_t>("thermalCorrectionE", contentOfFile.find("Thermal correction to Energy=")));
    positionsInFile.insert(pair<string, size_t>("thermalCorrectionS", contentOfFile.find("Thermal correction to Enthalpy=")));
    positionsInFile.insert(pair<string, size_t>("thermalCorrectionG", contentOfFile.find("Thermal correction to Gibbs Free Energy=")));
    positionsInFile.insert(pair<string, size_t>("sumElZE", contentOfFile.find("Sum of electronic and zero-point Energies=")));
    positionsInFile.insert(pair<string, size_t>("sumElTE", contentOfFile.find("Sum of electronic and thermal Energies=")));
    positionsInFile.insert(pair<string, size_t>("sumElTS", contentOfFile.find("Sum of electronic and thermal Enthalpies=")));
    positionsInFile.insert(pair<string, size_t>("sumElTG", contentOfFile.find("Sum of electronic and thermal Free Energies=")));
    
    for (map<string, size_t>::const_iterator pos = positionsInFile.begin(); pos != positionsInFile.end(); pos++) {
        if (pos->second == string::npos) {
            stringstream error;
            error << "Error whle reading the " << pos->first << endl;
            throw fatalGaussianError(error.str().c_str());
        }
    }
    
    //Read Data to class
    map<string, size_t>::const_iterator pos;
    size_t begOfValue;
    size_t endOfValue = contentOfFile.find("E (Thermal)", positionsInFile.find("ZPC")->second);
    //Read ZPC
    if (endOfValue == string::npos) {
        exit(1);
    }
    begOfValue = (contentOfFile.find("=", positionsInFile.find("ZPC")->second))+ 1 ;
    stringstream *iss = new stringstream(contentOfFile.substr(begOfValue, endOfValue-begOfValue));
    
    do {
        *iss >> ZPC;
    } while (!*iss);
    delete iss;
    
    //Read Thermal Correction Energy
    
    begOfValue = (contentOfFile.find("=", positionsInFile.find("thermalCorrectionE")->second))+ 1 ;
    iss = new stringstream(contentOfFile.substr(begOfValue,endOfValue-begOfValue));
    
    do {
        *iss >> thermalCorrectionE;
    } while (!*iss);
    delete iss;
    
    //Read Thermal correction to Enthalpy
    
    begOfValue = (contentOfFile.find("=", positionsInFile.find("thermalCorrectionS")->second))+ 1 ;
    iss = new stringstream(contentOfFile.substr(begOfValue,endOfValue-begOfValue));
    
    do {
        *iss >> thermalCorrectionS;
    } while (!*iss);
    delete iss;
    
    //Read Thermal correction to Gibbs Free Energy
    
    begOfValue = (contentOfFile.find("=", positionsInFile.find("thermalCorrectionG")->second))+ 1 ;
    iss = new stringstream(contentOfFile.substr(begOfValue,endOfValue-begOfValue));
    
    do {
        *iss >> thermalCorrectionG;
    } while (!*iss);
    
    delete iss;
    
    //Read Sum of electronic and zero-point

    begOfValue = (contentOfFile.find("=", positionsInFile.find("sumElZE")->second))+ 1 ;
    iss = new stringstream(contentOfFile.substr(begOfValue,endOfValue-begOfValue));
    
    do {
        *iss >> sumElZE;
    } while (!*iss);
    
    delete iss;
    
    // Read Sum of electronic and thermal
    
    begOfValue = (contentOfFile.find("=", positionsInFile.find("sumElTE")->second))+ 1 ;
    iss = new stringstream(contentOfFile.substr(begOfValue,endOfValue-begOfValue));
    
    do {
        *iss >> sumElTE;
    } while (!*iss);
    
    delete iss;
    
    //Read Sum of electronic and thermal
    
    begOfValue = (contentOfFile.find("=", positionsInFile.find("sumElTS")->second))+ 1 ;
    iss = new stringstream(contentOfFile.substr(begOfValue,endOfValue-begOfValue));
    
    do {
        *iss >> sumElTS;
    } while (!*iss);
    
    delete iss;
    
    //Read Sum of electronic and thermal Free
    
    begOfValue = (contentOfFile.find("=", positionsInFile.find("sumElTG")->second))+ 1 ;
    iss = new stringstream(contentOfFile.substr(begOfValue,endOfValue-begOfValue));
    
    do {
        *iss >> sumElTG;
    } while (!*iss);
    
    delete iss;
    
}
