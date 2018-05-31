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


#ifndef Tunneling_Analysis_IRC_Job_h
#define Tunneling_Analysis_IRC_Job_h

#include "GaussianOut.h"
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include "regex.h"
#ifndef Tunneling_Analysis_HelpFunctions_h
#include "help/HelpFunctions.h"
#endif



struct IRCPathElement{
    unsigned int step;
    double energy;
    double RxCoord;
    void clear(){
        step=0;
        energy=0.0;
        RxCoord=0.0;
    }
};

struct Geometry {
    int centerNumber;
    int atomicNumber;
    int atomicType;
    std::string isotope;
    double x;
    double y;
    double z;
};

//Used for bubble sort

enum sortOptions {
    fwdStep = 0x01,
    fwdEnergy = 0x02,
    fwdRxCoord = 0x04,
    revStep = 0x08,
    revEnergy = 0x10,
    revRxCoord = 0x20
    };

typedef std::vector<IRCPathElement> IRCPathVector;
typedef std::vector<Geometry> GeometryVector;
typedef std::vector<GeometryVector> molecule;
typedef std::map<double, GeometryVector> geometryMap;

class IRC_Job : public GaussianOut  {
    
    geometryMap molMap;
    IRCPathVector IRCPath;
    double TSEnergy;
    unsigned int numberOfSteps;
    bool isForward;
    long numberOfAtoms;
    
    bool initIRCJob(std::string& );
    
    bool getInputGeometry(const std::string& geometryString){
        GeometryVector InputStructure;
        size_t posOfInputGeom = geometryString.find("Input orientation:");
        if (posOfInputGeom != std::string::npos) {
            std::string sub = geometryString.substr(posOfInputGeom);
            size_t posOfGeomBeg = sub.find_first_of("1");
            std::stringstream iss(sub.substr(posOfGeomBeg));
            do {
                Geometry temp;
                iss >> std::skipws >> temp.centerNumber >> temp.atomicNumber >> temp.atomicType >> temp.x >> temp.y >> temp.z;
                if(iss)
                    temp.isotope=this->getAtomicSymbols().at(temp.centerNumber-1);
                InputStructure.push_back(temp);
            } while (iss);
            InputStructure.pop_back();
            molMap.insert(std::pair<double, GeometryVector>(0.0,InputStructure));
            return true;
        }
        return false;
    }
    
    void readIRC(const std::string &s){
        // First of all let's get the TS energy
        getTSEnergy(s);
        //Get Input Structure (Push_back because vector is empty)
        GeometryVector inputGeometry;
        if (!getInputGeometry(s)) {
            throw warningGaussianError("Warning: An error ocurred during input geometry reading");
        }

       
        // Now for the mp/hybrid energies we need another method
        bool isMP=false;
        std::string cMethod=this->getCalculationMethod();
        std::transform(cMethod.begin(), cMethod.end(), cMethod.begin(), ::tolower);
        if(cMethod.find("mp") != std::string::npos)
            isMP=true;
        if(cMethod.find("b2plyp") != std::string::npos)
            isMP=true;
        if(cMethod.find("mpw2plyp") != std::string::npos)
            isMP=true;


        // Let's find the bounderies where the point on IRC is optimized

        
        std::string line;
        std::istringstream iss(s);
        IRCPathElement ie;
        GeometryVector gv;
        while (std::getline(iss, line)) {
            boost::smatch what;

            if(boost::regex_search(line,what,regex::gaussian::geometry,
            		boost::match_extra)){
                Geometry g;
                g.centerNumber=std::stoi(what[1]);
                //g.atomicNumber=std::stoi(what[2]);
                g.atomicNumber=std::stoi(what[2]);

                g.atomicType=std::stoi(what[3]);
                g.x=std::stod(what[4]);
                g.y=std::stod(what[5]);
                g.z=std::stod(what[6]);
                g.isotope=this->getAtomicSymbols().at(g.centerNumber-1);
                gv.push_back(g);
            }
            
            else if(boost::regex_search(line,what,regex::gaussian::scfEnergy,
            		boost::match_extra) && !isMP){
                ie.energy=std::stod(what[1]);
                ie.energy=ie.energy-this->TSEnergy;
            }
            
            else if(boost::regex_search(line,what,regex::gaussian::mpEnergy,
            		boost::match_extra) && isMP){
                std::string s=what[1];
                std::replace(s.begin(),s.end(),'D','E');
                ie.energy=std::stod(s);
                ie.energy=ie.energy-this->TSEnergy;
            }
            // Rcoord should be last entry of the opt cycle so push back
            else if (boost::regex_search(line,what,regex::gaussian::reactionCoord,
            		boost::match_extra)) {
                ie.RxCoord=std::stod(what[1]);
                if(!isForward)
                    ie.RxCoord=-ie.RxCoord;
                
                this->IRCPath.push_back(ie);
                GeometryVector nrGv;
                for (auto rIt=gv.rbegin(); rIt != gv.rend(); rIt++) {
                    nrGv.push_back(*rIt);
                    if (rIt->centerNumber == 1)
                        break;
                }
                
                
                // Now put geometry back in right order and push back
#ifdef CPP_17
                this->molMap.try_emplace(ie.RxCoord,GeometryVector(nrGv.rbegin(),nrGv.rend()));
#else
                this->molMap.emplace(ie.RxCoord,GeometryVector(nrGv.rbegin(),nrGv.rend()));
                gv.clear();
                ie.clear();
#endif
            }
            
            else if(boost::regex_search(line,what,regex::gaussian::pointNumber,
            		boost::match_extra)){
                ie.step=std::stoi(what[1]);
            }
    }

        

    }
    
    void getTSEnergy(std::string inputString){
        // There are two possiblities at the beginning:
        // Poss 1: rcfc ( Energy and FC are read from chk file )
        boost::regex rcfc("[Rr][Cc][Ff][Cc]");
        std::string cType=this->getCalculationType();
        if (boost::regex_search(inputString, rcfc)) {
            boost::regex energyR("\\s+Energy From Chk\\s+=\\s+([+-]?\\d+\\.?\\d+)");
            boost::smatch what;
            if(boost::regex_search(inputString, what,energyR,boost::match_extra)){
                help::to_number<double>(this->TSEnergy, what[1]);
            }
            else
                throw fatalGaussianError("Could not read TS energy");
        } else{
            // TO DO: Check regex here for case of b2blyp
            // Poss 2: scf is done first ( First "SCF Done:" or RMP* contains energy )
            boost::regex scfEnergy("^\\s+SCF Done:\\s+E\\(\\w+\\)\\s+=\\s+([+-]?\\d+\\.?\\d+)");
            boost::smatch what;
            // Now for the mp energies we need another method
            // First let't find the mp2 number:
            bool isMP=false;
            std::string mpNum("2");
            boost::regex mpReg("[Mm][Pp](\\d+)");
            if(boost::regex_search(this->getCalculationMethod(),what,mpReg,boost::match_extra)){
                mpNum=what[1];
                isMP=true;
            }
            const boost::regex mp2Energy("EUMP" + mpNum + "\\s+=\\s+([+-]?\\d+\\.\\d+D?[+-]?\\d+)");

            if(boost::regex_search(inputString,what,scfEnergy,boost::match_extra) && !isMP){
                this->TSEnergy=std::stod(what[1]);
            } else if(boost::regex_search(inputString,what,mp2Energy,boost::match_extra) && isMP){
                std::string s=what[1];
                std::replace(s.begin(),s.end(),'D','E');
                this->TSEnergy=std::stod(s);
            }
            else
                throw fatalGaussianError("Could not read TS energy");
        }
        IRCPathElement ts;
        ts.energy=0.0;
        ts.step=0;
        ts.RxCoord=0.0;
        this->IRCPath.push_back(ts);
    }

    
public:
    
    IRC_Job(const std::string& filename);
    IRC_Job(): GaussianOut(){
        
    }
    
    bool isIRCPathForward() const;
    void display() const;
    double getTSEnergy() const{
        return this->TSEnergy;
    }
    const long getNumberOfAtoms() const{
        return this->numberOfAtoms;
    }
    std::string getGeometryAtRxCoord(const double&, bool satoms=false) const;
    std::fstream& getGeometryAtRxCoord(std::fstream&, const double&, bool) const;
    const IRCPathVector& getIRCPathVector() const{
        return this->IRCPath;
    }
    bool writeIRCToFile(std::fstream& outFile) const;
    bool init(std::string);
    bool init(const char*);
    void deleteLastElementOfIRC(){
        this->IRCPath.pop_back();
    }
    void sortIRCPath(sortOptions options){
    	// TO DO: A test for a multiple set of bits must be implemented
            if (options & fwdStep) {
                int j = 0;
                bool swapped = true;
                while (swapped) {
                        
                    swapped = false;
                    j++;
                    IRCPathElement tmp;
                        
                    for (unsigned int i = 0; i < this->IRCPath.size() - j; i++) {
                            
                        if (this->IRCPath[i].step > this->IRCPath[i + 1].step) {
                                
                            tmp = this->IRCPath[i];
                                
                            this->IRCPath[i] = this->IRCPath[i + 1];
                                
                            this->IRCPath[i + 1] = tmp;
                                
                            swapped = true;
                                
                        }
                    }
                }
            }
            else if (options & fwdEnergy) {
                int j = 0;
                bool swapped = true;
                while (swapped) {
                    
                    swapped = false;
                    j++;
                    IRCPathElement tmp;
                    
                    for (unsigned int i = 0; i < this->IRCPath.size() - j; i++) {
                        
                        if (this->IRCPath[i].energy > this->IRCPath[i + 1].energy) {
                            
                            tmp = this->IRCPath[i];
                            
                            this->IRCPath[i] = this->IRCPath[i + 1];
                            
                            this->IRCPath[i + 1] = tmp;
                            
                            swapped = true;
                            
                        }
                    }
                }
            }
            else if (options & fwdRxCoord) {
                int j = 0;
                bool swapped = true;
                while (swapped) {
                    
                    swapped = false;
                    j++;
                    IRCPathElement tmp;
                    
                    for (unsigned int i = 0; i < this->IRCPath.size() - j; i++) {
                        
                        if (this->IRCPath[i].RxCoord > this->IRCPath[i + 1].RxCoord) {
                            
                            tmp = this->IRCPath[i];
                            
                            this->IRCPath[i] = this->IRCPath[i + 1];
                            
                            this->IRCPath[i + 1] = tmp;
                            
                            swapped = true;
                            
                        }
                    }
                }
            }
            else if (options & revStep) {
                int j = 0;
                bool swapped = true;
                while (swapped) {
                    
                    swapped = false;
                    j++;
                    IRCPathElement tmp;
                    
                    for (unsigned int i = 0; i < this->IRCPath.size() - j; i++) {
                        
                        if (this->IRCPath[i].step < this->IRCPath[i + 1].step) {
                            
                            tmp = this->IRCPath[i];
                            
                            this->IRCPath[i] = this->IRCPath[i + 1];
                            
                            this->IRCPath[i + 1] = tmp;
                            
                            swapped = true;
                            
                        }
                    }
                }
            }
            else if (options & revEnergy) {
                int j = 0;
                bool swapped = true;
                while (swapped) {
                    
                    swapped = false;
                    j++;
                    IRCPathElement tmp;
                    
                    for (unsigned int i = 0; i < this->IRCPath.size() - j; i++) {
                        
                        if (this->IRCPath[i].energy < this->IRCPath[i + 1].energy) {
                            
                            tmp = this->IRCPath[i];
                            
                            this->IRCPath[i] = this->IRCPath[i + 1];
                            
                            this->IRCPath[i + 1] = tmp;
                            
                            swapped = true;
                            
                        }
                    }
                }
            }
            else if (options & revRxCoord) {
                int j = 0;
                bool swapped = true;
                while (swapped) {
                        
                    swapped = false;
                    j++;
                    IRCPathElement tmp;
                    
                    for (unsigned int i = 0; i < this->IRCPath.size() - j; i++) {
                        if (this->IRCPath[i].RxCoord < this->IRCPath[i + 1].RxCoord) {
                            
                            tmp = this->IRCPath[i];
                                
                            this->IRCPath[i] = this->IRCPath[i + 1];
                                
                            this->IRCPath[i + 1] = tmp;
                                
                            swapped = true;
                                
                        }
                    }
                }
            }
                
            return;
        }
    
    friend std::ostream& operator<<(std::ostream& out, IRC_Job& IRCOut);

            
};
            
inline std::ostream& operator<<(std::ostream& out, IRC_Job& IRCOut){
    for (geometryMap::const_iterator pos = IRCOut.molMap.begin(); pos != IRCOut.molMap.end(); pos++) {
        std::string spacer(30, '-');
        out << spacer << std::endl;
        out << "Geometry at step: " << pos->first << std::endl;
        out << spacer << std::endl;
        for (GeometryVector::const_iterator pos2 = pos->second.begin(); pos2 < pos->second.end(); pos2++) {
            out << std::setprecision(6) << std::fixed << std::setw(10) << std::setfill(' ') << pos2->x << std::setw(10) << std::setfill(' ') << pos2->y << std::setw(10) << std::setfill(' ') << pos2->z << std::endl;
        }
    }
    out << "\n \n Summary of the IRC:" << " " << std::endl;
    out << std::setfill(' ') << std::setw(15) << "Step Number " << std::setfill(' ')<< std::setw(10) << "Energy " << std::setfill(' ') << std::setw(12) << std::right << "Rx Coord" << std::endl;
    out << std::setw(50) << std::setfill('-') << " " << std::endl;
    for (IRCPathVector::const_iterator pos=IRCOut.IRCPath.begin(); pos < IRCOut.IRCPath.end(); pos++) {
        out << std::setfill(' ') << std::setw(15) << std::left << pos->step << " " << std::setprecision(6) << std::fixed << std::setw(10) << std::setfill(' ') << std::right << pos->energy << " " << std::setw(10) << std::setfill(' ') << std::right << pos->RxCoord << std::endl;
    }
    out << std::setw(50) << std::setfill('-') << " " << std::endl;
    
    return out;
                
}
        


#endif
