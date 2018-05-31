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


#include "FBIRC.h"
#include <string>
#include <iostream>

using namespace std;

FBIRC::FBIRC(){
    this->IRC = new IRC_Job[2];
}

FBIRC::FBIRC(const string& forwardIRCPathname, const string& reverseIRCPathname){
    this->IRC = NULL;
    if (!init(forwardIRCPathname,reverseIRCPathname)) {
		throw fatalGaussianError(
				"An error occurred during initialization of FBIRC");
    }
   
}

bool FBIRC::init(const string& forwardIRCPathname, const string& reverseIRCPathname){
    if(this->IRC == NULL)
        this->IRC = new IRC_Job[2];
    if (!(IRC)->init(reverseIRCPathname)) {
        return false;
    }
    if(!(IRC + 1)->init(forwardIRCPathname))
        return false;
    
    //Check if somone submitted two forward or reverse calculations
    if (IRC->isIRCPathForward() == (IRC + 1)->isIRCPathForward()) {
        throw fatalGaussianError("ERROR: Both files are calculated in the same direction by gaussian");
        return false;
    }
    
    //Check if order is correct
    if (IRC->isIRCPathForward()) {
        throw fatalGaussianError("The second argument is not a reverse IRC");
        return false;
    }
    
    //Sort IRC path forward if it is the reverse path and write the IRC to the vector. Also delete the last entry after sorting, because it is the TS
    for (int i = 0; i < 2; i++) {
        IRCPathVector IRCPath = (IRC + i)->getIRCPathVector();
        if (!(IRC + i)->isIRCPathForward()) {
            (IRC + i)->sortIRCPath(fwdRxCoord);
            //(IRC + i)->deleteLastElementOfIRC();
        }
        
        if (theIRC.empty()) {
            theIRC = IRCPath;
        }
        
        else{
            for (IRCPathVector::const_iterator pos = IRCPath.begin(); pos < IRCPath.end(); pos++) {
                theIRC.push_back(*pos);
            }
        }
    }

    return true;
    
}
