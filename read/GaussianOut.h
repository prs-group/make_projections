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


#ifndef Tunneling_Analysis_GaussianOut_h
#define Tunneling_Analysis_GaussianOut_h

#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <exception>
#include <vector>
#ifdef __GNUC__
#include <cstdlib>
#endif


struct CaMUL {
    int charge;
    int multiplizity;
};


class GaussianOut{
private:
    std::string calcMethod;
    CaMUL chargeAndMultiplizity;
    bool errorTermination;
    std::string filename;
    std::fstream outputFile;
    std::string calcType;
    std::string jobname;
    std::string initialGeom;
    // This is used for isotope support (iso=)
    std::vector<std::string> atomicSymbols;
    
    void readInitialGeom();
    
public:
    GaussianOut(const std::string&);
    GaussianOut(){
        errorTermination = true;
        chargeAndMultiplizity.charge = 0;
        chargeAndMultiplizity.multiplizity=0;
#ifdef DEBUG
        std::cout << "Warning: Please use the init function to initialize this object" << std::endl;
#endif
    }
    virtual ~GaussianOut(){
        if (outputFile.is_open()) {
            outputFile.close();
        }
    }
    bool didCalculationFinishProperly() const;
    std::string getCalculationMethod() const;
    std::string getFilename() const;
    std::string getCalculationType() const;
    const CaMUL getChargeAndMultiplicity() const{
        return chargeAndMultiplizity;
    }
    const int getCharge() const{
        return chargeAndMultiplizity.charge;
    }
    const int getMultiplizity() const{
        return chargeAndMultiplizity.multiplizity;
    }
    
    const std::string getJobname() const{
        return jobname;
    }
    
    const std::vector<std::string> getAtomicSymbols() const {
        return this->atomicSymbols;
    }
    
    //The init function can be used to initialize an object constructed by the default constructor
    virtual bool init(const std::string);
    virtual bool init(const char*);
    
    virtual bool operator==(const GaussianOut&) const;
    virtual bool operator==(const std::string&) const;
    virtual bool operator!=(const GaussianOut&) const;
    virtual bool operator!=(const std::string&) const;
    
    friend std::ostream& operator<<(std::ostream& out, GaussianOut& gOut){
        if (gOut.errorTermination) {
            out << "You're submitted file contains an error termination" << std::endl;
            return out;
        }
        out << std::setw(40) << "You submitted the file: " << gOut.filename << std::endl;
        out << std::setw(40) << "The calculation method was: " << gOut.calcMethod << std::endl;
        out << std::setw(40) << "The calculation type you used was: " << gOut.calcType << std::endl;
        out << std::setw(40) << "Your molecule has an charge of: " << gOut.getCharge() << " and an multiplicity of: " << gOut.getMultiplizity() << std::endl;
        
        
        
        return out;
    }
    
};
        
class fatalGaussianError : public std::exception{
private:
    const char* msg;
public:
    fatalGaussianError(): msg("An internal error occured"){
    }
    fatalGaussianError(const char* message):msg(message){
}
    const char* what() const throw(){
        return msg;
    }
};

class warningGaussianError : public std::exception{
private:
    const char* msg;
public:
    warningGaussianError():msg("A warning occured during reading"){
    }
    warningGaussianError(const char* message):msg(message){
    }
    const char* what() const throw(){
        return msg;
    }
};


#endif
