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
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <boost/regex.hpp>
#include "IRC_Job.h"
#include "../options/options.h"
#ifndef Tunneling_Analysis_HelpFunctions_h
#define Tunneling_Analysis_HelpFunctions_h
#include "../help/HelpFunctions.h"
#endif
#include "config.h"
#include "config/Config.h"

using namespace std;
using namespace myOptions;

IRC_Job::IRC_Job(const string& filename): GaussianOut(filename){
    if(!this->init(filename)){
		throw warningGaussianError(
				"An error occurred, while initialize the IRC_Job object");
    }
}
bool IRC_Job::isIRCPathForward() const{
    return this->isForward;
}

bool IRC_Job::initIRCJob(std::string& filename){
    if (!this->didCalculationFinishProperly() && !options::force()){
        throw fatalGaussianError("Your calculation did not finish properly");
        return false;
    }
    
    if (!this->didCalculationFinishProperly() && options::force()){
        cout << "Warning: Force is enabled and you're calculation did not finish properly" << std::endl;
    }
    
    if (this->getCalculationMethod().empty()) {
        throw fatalGaussianError("Error reading the type of calculation");
        return false;
    }
    fstream IRCFile(filename);
    std::string calcType=this->getCalculationType();
    boost::match_results<std::string::const_iterator> what;
    boost::regex IRCEx("[Ii][Rr][Cc]");
    std::string::const_iterator start,end;
    start=calcType.begin();
    end=calcType.end();
    if (!boost::regex_search(start,end,what,IRCEx,boost::match_default)) {
        throw fatalGaussianError("Sorry you did not submit an IRC calculation");
        return false;
    }
    this -> isForward = false;
    boost::regex forwardEx("[Ff][Oo][Rr][Ww][Aa][Rr][Dd]");
    
    if (regex_search(start,end,what,forwardEx,boost::match_default)) {
        this->isForward = true;
    }
    if (!IRCFile.is_open()) {
        IRCFile.open(this->getFilename().c_str(),std::ios::in);
    }
    std::string contentOfFile;
    help::readFileToString(IRCFile, contentOfFile);
    readIRC(contentOfFile);
    //Get Number of Atoms
    numberOfAtoms = molMap.size();
    //In the case of an reverse file sort IRC forward in order to sync geometry with IRC
    numberOfSteps = IRCPath.back().step;
    if (!this->isIRCPathForward()) {
        this->sortIRCPath(revRxCoord);
    }
    return true;
    
    
    // Old code
    
    /*size_t posOfIRCpath = contentOfFile.find("Summary of reaction path following");
    if (posOfIRCpath == std::string::npos) {
        throw fatalGaussianError("Could not find summary of reaction path");
    }
    std::string IRCPathbeginning = contentOfFile.substr(posOfIRCpath,fileSize);
    size_t pos=IRCPathbeginning.find_first_not_of("\n");
    size_t lastPos=IRCPathbeginning.find_first_of("\n");
    for(int i=0; i < 4; i++){
        lastPos=IRCPathbeginning.find_first_not_of("\n",pos);
        pos=IRCPathbeginning.find_first_of("\n",lastPos);
    }
    IRCPathbeginning.erase(0,lastPos);
    size_t posOfIRCbeginning = IRCPathbeginning.find_first_of("1");
     IRCPathbeginning.erase(0, posOfIRCbeginning);
    std::stringstream iss(IRCPathbeginning);
    
    char line[1000];
    
    do {
     IRCPathElement temp;
     iss >> temp.step >> temp.energy >>temp.RxCoord;
     IRCPath.push_back(temp);
     } while (iss);*/
    /*boost::regex numberRegex("^\\s+(\\d+)\\s+([-+]?[0-9]*\\.?[0-9]+)\\s+([-+]?[0-9]*\\.?[0-9]+)");
    while(iss.getline(line,1000)){
        IRCPathElement temp;
        string l(line);
        boost::smatch match;
        start=l.begin();
        end=l.end();
        if(boost::regex_search(start,end,match,numberRegex,boost::match_default)){
            if(match.size() != 4)
                throw fatalGaussianError("You're IRC in the gaussian files seems to be malformed");
            try{
                temp.step=help::stringToDouble(match[1]);
                temp.energy=help::stringToDouble(match[2]);
                temp.RxCoord=help::stringToDouble(match[3]);
                IRCPath.push_back(temp);
            } catch(invalid_argument &e){
                cerr << "A problem occured during conversion to double: " << e.what() << endl;
                exit(1);
            } catch(runtime_error &e){
                cerr << "Runtime Error during conversion to double: " << e.what() << endl;
                exit(1);
            } catch(...){
                cerr << "A fatal error occured during conversion to double" << endl;
                exit(1);
            }
        } else
            break;
        if(!boost::regex_search(start,end,match,numberRegex,boost::match_default))
         break;
         exit(1);
         // Split string
         vector<string> split;
         help::tokenize(l,split);
         if(split.size() <= 3)
         throw fatalGaussianError("You're IRC in the gaussian files seems to be malformed");
         try{
         temp.step=help::stringToDouble(split[0]);
         temp.energy=help::stringToDouble(split[1]);
         temp.RxCoord=help::stringToDouble(split[2]);
         } catch(invalid_argument &e){
         cerr << "A problem occured during conversion to double: " << e.what() << endl;
         exit(1);
         } catch(runtime_error &e){
         cerr << "Runtime Error during conversion to double: " << e.what() << endl;
         exit(1);
         } catch(...){
         cerr << "A fatal error occured during conversion to double" << endl;
         exit(1);
         }
        
    }
    
    

    

    
    

    //Read all Geometries

    //readGeometry(contentOfFile);
    

    numberOfAtoms = 1;
     while (pos->centerNumber != 1) {
     numberOfAtoms++;
     pos++;
     }
    
    //Get TS Energy
    getTSEnergy(contentOfFile);
    return true;*/

}


bool IRC_Job::init(string filename){
    GaussianOut::init(filename);
    return this->initIRCJob(filename);
}

bool IRC_Job::init(const char* filename){
    GaussianOut::init(filename);
    string f(filename);
    return this->initIRCJob(f);
}
void IRC_Job::display() const{
    if (this->isIRCPathForward()) {
        cout << "The IRC is calculated forward" << endl;
    }
    else {
        cout << "The IRC is calculated reverse" << endl;
    }
    cout.precision(6);
    cout << setfill(' ') <<"Step" << setw(12) << "Energy" << setw(12) << "Rx Coord" << endl;
    IRCPathVector::const_iterator pos;
    for (pos = this->IRCPath.begin(); pos < this->IRCPath.end(); pos++) {
        cout << fixed << pos->step << setw(12) << pos->energy << setw(12) << pos->RxCoord << endl;
    }
    for (geometryMap::const_iterator pos = this->molMap.begin(); pos != this->molMap.end(); pos++) {
        string spacer(30, '-');
        cout << spacer << endl;
        cout << "Geometry at step: " << pos->first << endl;
        cout << spacer << endl;
        for (GeometryVector::const_iterator pos2 = pos->second.begin(); pos2 < pos->second.end(); pos2++) {
            cout << setprecision(6) << fixed << setw(10) << setfill(' ') << pos2->x << setw(10) << setfill(' ') << pos2->y << setw(10) << setfill(' ') << pos2->z << endl;
        }
    }
}

string IRC_Job::getGeometryAtRxCoord(const double& rxCoord,bool satoms) const{
    stringstream iss;
    geometryMap::const_iterator pos = this->molMap.find(rxCoord);
    for (GeometryVector::const_iterator posGeom = pos->second.begin(); posGeom < pos->second.end(); posGeom++) {
        if (!satoms)
            iss << left << setprecision(6) << fixed << setfill(' ') << setw(10) << posGeom->atomicNumber << setw(10) <<  posGeom->x << setw(10) << posGeom->y << setw(10) << posGeom->z << endl;
        else{
            Config conf;
            const pseMap pse=conf.getPSE();
            iss << left << setprecision(6) << fixed << setfill(' ') << setw(10) << pse.at(posGeom->atomicNumber) << setw(10) <<  posGeom->x << setw(10) << posGeom->y << setw(10) << posGeom->z << endl;
            
        }
    }
    return iss.str();
}

fstream& IRC_Job::getGeometryAtRxCoord(fstream& out, const double& rxCoord,bool isotopes) const{
    geometryMap::const_iterator pos = this->molMap.find(rxCoord);
    for (GeometryVector::const_iterator posGeom = pos->second.begin(); posGeom < pos->second.end(); posGeom++) {
        if (isotopes)
               out << left << setprecision(6) << fixed << setfill(' ') << setw(10) << posGeom->isotope << setw(10) <<  posGeom->x << setw(10) << posGeom->y << setw(10) << posGeom->z << endl;
        else
               out << left << setprecision(6) << fixed << setfill(' ') << setw(10) << posGeom->atomicNumber << setw(10) <<  posGeom->x << setw(10) << posGeom->y << setw(10) << posGeom->z << endl;
    }
    return out;
}
bool IRC_Job::writeIRCToFile(fstream& outFile) const{
    if (!outFile.is_open()) {
        throw fatalGaussianError("The file is not valid");
        return false;
    }
    
    for (IRCPathVector::const_iterator pos = this->IRCPath.begin(); pos < this->IRCPath.end(); pos++) {
        outFile << setprecision(6) << fixed << setw(10) << setfill(' ') << pos->RxCoord << setw(10) << pos->energy << endl;
    }
    
    return true;
}
