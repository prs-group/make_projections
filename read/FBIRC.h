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


#ifndef Tunneling_Analysis_FBIRC_h
#define Tunneling_Analysis_FBIRC_h
#include <string>
#include <fstream>
#include "IRC_Job.h"


class FBIRC {
    IRC_Job* IRC;
    IRCPathVector theIRC;
    
public:
    FBIRC();
    FBIRC(const std::string&, const std::string&);
    bool init(const std::string&, const std::string&);

    const IRC_Job* getIRC_Job() const{
        return this->IRC;
    }

    friend std::ostream& operator<<(std::ostream& out, FBIRC& aFBIRC){
        const IRC_Job* IRCOut = new IRC_Job[2];
        IRCOut = aFBIRC.getIRC_Job();
        for(int i = 0; i < 2; i++){
            for (IRCPathVector::const_iterator pos=(IRCOut + i)->getIRCPathVector().begin(); pos < (IRCOut + i)->getIRCPathVector().end(); pos++) {
                out << std::setfill(' ') << std::setw(15) << std::left << std::setprecision(6) << std::fixed << std::setw(10) << std::setfill(' ') << std::right << pos->RxCoord << " " << std::setw(10) << std::setfill(' ') << std::right << pos->energy << std::endl;
                }
        }
                delete [] IRCOut;
                return out;
    }
                
    friend std::fstream& operator<<(std::fstream& out, FBIRC& aFBIRC){
        
        if(!out.is_open()){
            return out;
        }
        
        const IRC_Job* IRCOut = new IRC_Job[2];
        IRCOut = aFBIRC.getIRC_Job();
        for(int i = 0; i < 2; i++){
            for (IRCPathVector::const_iterator pos=(IRCOut + i)->getIRCPathVector().begin(); pos < (IRCOut + i)->getIRCPathVector().end(); pos++) {
                out << std::setfill(' ') << std::setw(15) << std::left << std::setprecision(6) << std::fixed << std::setw(10) << std::setfill(' ') << std::right << pos->RxCoord << " " << std::setw(10) << std::setfill(' ') << std::right << (pos->energy + IRCOut->getTSEnergy()) << std::endl;
            }
        }
        delete [] IRCOut;
        return out;
    }
};

#endif
