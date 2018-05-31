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


#ifndef Tunneling_Analysis_FreqProjectedJob_h
#define Tunneling_Analysis_FreqProjectedJob_h

#include "GaussianOut.h"
#include <string>
#include <iomanip>
#ifdef __GNUC__
#include <cstdlib>
#endif


class FreqProjectedJob: public  GaussianOut{
    
    double ZPC;                //The Zero-point correction in (Hatree/Particle)
    double thermalCorrectionE; // The Thermal correction to Energy
    double thermalCorrectionS; // The Thermal correction to Enthalpy
    double thermalCorrectionG; // The Thermal correction to Gibbs Free Energy 
    double sumElZE;            // The Sum of electronic and zero-point Energies
    double sumElTE;            // The Sum of electronic and thermal Energies
    double sumElTS;            // The Sum of electronic and thermal Enthalpies
    double sumElTG;            // The Sum of electronic and thermal Free Energies
    
public:
    FreqProjectedJob();
    FreqProjectedJob(const std::string&);
    virtual ~FreqProjectedJob() {
        
    }
    
    const double getZPC() const{
        return this->ZPC;
    }
    
    const double getthermalCorrectionE() const{
        return this->thermalCorrectionE;
    }
    
    const double getthermalCorrectionS() const{
        return this->thermalCorrectionS;
    }
    
    const double getthermalCorrectionG() const{
        return this->thermalCorrectionG;
    }
    
    const double getsumOfElAndZPE() const{
        return this->sumElZE;
    }
    
    const double getsumOfElAndTE() const {
        return this->sumElTE;
    }
    
    const double getsumOfElAndTS() const {
        return this->sumElTS;
    }
    
    const double getsumOfElAndTG() const {
        return this->sumElTG;
    }
    
    friend std::ostream& operator<<(std::ostream& out, FreqProjectedJob& FreqJob){
        out << std::setprecision(6) << std::fixed;
        out << std::setfill(' ') << std::setw(50) << "Zero-point correction = " << FreqJob.ZPC << std::endl;
        out << std::setfill(' ') << std::setw(50) << "Thermal correction to Energy = " << FreqJob.thermalCorrectionE << std::endl;
        out << std::setfill(' ') << std::setw(51) << "Thermal correction to Enthalpy =  " << FreqJob.thermalCorrectionS << std::endl;
        out << std::setfill(' ') << std::setw(50) << "Thermal correction to Gibbs Free Energy = " << FreqJob.thermalCorrectionG << std::endl;
        out << std::setfill(' ') << std::setw(50) << "Sum of electronic and zero-point Energies = " << FreqJob.sumElZE << std::endl;
        out << std::setfill(' ') << std::setw(50) << "Sum of electronic and thermal Energies = " << FreqJob.sumElTE << std::endl;
        out << std::setfill(' ') << std::setw(50) << "Sum of electronic and thermal Enthalpies = " << FreqJob.sumElTS << std::endl;
        out << std::setfill(' ') << std::setw(50) << "Sum of electronic and thermal Free Energies = " << FreqJob.sumElTG << std::endl;
        
        return out;
    }
    
};

#endif
