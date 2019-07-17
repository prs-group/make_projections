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
#include <fstream>
#include <streambuf>
#include <sstream>
#include "GaussianOut.h"
#include <string>
#include "regex.h"
#include <boost/regex.hpp>
#include "../options/options.h"


#include "../help/HelpFunctions.h"



using namespace std;
using namespace myOptions;

//Constructor Definition

GaussianOut::GaussianOut(const string& filename): outputFile(filename.c_str(),ios::in){
    if(!init(filename)){
        throw fatalGaussianError("Your file cannot be read");
    }
}

bool GaussianOut::init(const string filename){
    
    const char* c_filename = filename.c_str();
    if(GaussianOut::init(c_filename)){
        return true;
    }

    return false;
    
}

bool GaussianOut::init(const char* filename){
    fstream GaussoutFile(filename);
    if (!GaussoutFile.good()) {
        throw fatalGaussianError("Sorry I cannot read your file: please enter a valid path");
        return false;
    }
    this->filename=filename; 
    errorTermination = true;
    if(!GaussoutFile.is_open())
        GaussoutFile.open(this->filename.c_str(),ios::in);
    string contentOfFile;
    help::readFileToString(GaussoutFile, contentOfFile);
    
    //Gaussian File looks like
    //-------------------
    //#calcMethod calcSet
    // calcsetcontinue
    //-------------------
    
    if (contentOfFile.find("Normal termination")!= string::npos || options::force()) {
        this->errorTermination = false;
    }
    if (!this->errorTermination || options::force()) {
        size_t begOfSearch = contentOfFile.find_first_of("*"); //This is made due to a possible bug, that someone uses an # as an input filename
        size_t posOfCalctype = contentOfFile.find_first_of("#",begOfSearch);
        size_t posOfCalctypeEnding = contentOfFile.find("--", posOfCalctype);
        string calcTypeandCalcmethod = contentOfFile.substr(posOfCalctype,posOfCalctypeEnding-posOfCalctype);
        stringstream iss(calcTypeandCalcmethod);
        bool foundCalcMethod=false;
        do {
            string sub;
            iss >> sub;
            if (sub.length() > 2) { //Exclude #p and so on
                if (!foundCalcMethod) { //Uses the fact, that the calcmethod is first in line
                    this->calcMethod = sub;
                    foundCalcMethod=true;
                    continue;
                }
                this->calcType.append(sub);
                
            }
            
        } while (iss);
        
        if (this->calcMethod.find_first_of("#")!=string::npos) {
            this->calcMethod.erase(1); //Lösche "#"
        }
        size_t posOfNewline=this->calcType.find("\n");
        if (posOfNewline != string::npos) {
            this->calcType.erase(posOfNewline,1);
        }
    }
    else {
        this->calcType = "";
        this->calcMethod="";
    }
    
    //Getting the charge and multiplicity
    
    size_t posOfCharge = contentOfFile.find("Charge");
    if (posOfCharge != string::npos) {
        string substring = contentOfFile.substr(posOfCharge);
        size_t EndOfLine = substring.find_first_of("\n");
        substring = substring.substr(0,EndOfLine);
        size_t posOfCharge = substring.find_first_of("=");
        size_t posOfMul = substring.find_last_of("=");
        stringstream iss(substring.substr(posOfCharge+1));
        do {
            iss >> chargeAndMultiplizity.charge;
        } while (iss);
        
        stringstream iss2(substring.substr(posOfMul+1));
        do {
            iss2 >> chargeAndMultiplizity.multiplizity;
        } while (iss);
        
        //Getting the jobname
        
        // The jobname is enclosed by "-"
        // Before the jobname the method line is enclosed by "-"
        // So, count up to 5 to get the jobname
        // The jobname can also be multiline so stop parsing after 4

        jobname="";
        stringstream content(contentOfFile);
        const boost::regex l(R"(\s+[-]+)");
        int counter=0;
        constexpr int stop=5;
        char *line=new char[1000];
        bool startParsing=false;
        while( content.getline(line,1000)){
            if (boost::regex_match(line,l)){
                counter++;
                if(counter==stop)
                    startParsing=true;
                continue;
            } else {
                if(counter==stop+1)
                    break;
            }


            if (startParsing) {
                jobname+=line;
            }

        }

        this->readInitialGeom();
        delete [] line;
        
        GaussoutFile.close();
        
        return true;
    }
    
    
    
    
    GaussoutFile.close();
    return false;
}

void GaussianOut::readInitialGeom(){
    std::fstream outFile(filename,ios::in);
    if (!outFile.is_open()) {
        exit(1);
    }
    
    // note: the following loop terminates when std::ios_base::operator bool()
    // on the stream returned from getline() returns false
    bool startReading=false;
    for (std::array<char, 1000> a; outFile.getline(&a[0], 1000, '\n'); ) {
        string line(&a[0]);
        // When reading from checkpoint
        if (line.find(" Recover connectivity") != string::npos) {
            break;
        }
        if (line.find("Symbolic Z-matrix") != string::npos
            // If you start from checkpoint file
            || line.find("Redundant internal") != string::npos ||
            line.find("cartesian coordinates used") != string::npos) {
            startReading=true;
        }

        if (startReading) {
            if (line.empty())
                break;

			this->initialGeom.append(line + "\n");
        }
    }
    if(initialGeom.empty())
		throw fatalGaussianError("Could not read initial Geometry");
    size_t firstNewline=initialGeom.find("\n") + 1;
    initialGeom.erase(0, firstNewline);
//    cout << initialGeom << endl;
    // Get the atomic symbols with isotopes from geometry
    boost::smatch what;
    string line;
    stringstream iss(initialGeom);
    while (getline(iss,line)) {
        if (boost::regex_search(line,what,regex::gaussian::atomicSymbols,
        		boost::match_extra)) {
            std::string atSymbol="";
            for(auto s=what.begin()+1; s < what.end();s++){
                atSymbol.append(*s);
            }
            this->atomicSymbols.push_back(atSymbol);
        }
    }
}



bool GaussianOut::didCalculationFinishProperly() const{
    return !this->errorTermination;
}

string GaussianOut::getFilename() const {
    return this->filename;
}

string GaussianOut::getCalculationMethod() const {
    return this->calcMethod;
}

string GaussianOut::getCalculationType() const{
    return this->calcType;
}

bool GaussianOut::operator==(const GaussianOut &other) const{
    return (this->getFilename() == other.getFilename());
}

bool GaussianOut::operator==(const string &other) const{
    return (this->getFilename() == other);
}

bool GaussianOut::operator !=(const GaussianOut &other) const{
    return (this->getFilename() != other.getFilename());
}

bool GaussianOut::operator!=(const std::string &other) const{
    return (this->getFilename() != other);
}

