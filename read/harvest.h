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


#ifndef Tunneling_Analysis_harvest_h
#define Tunneling_Analysis_harvest_h

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <exception>
#include "options/options.h"
#include "FreqProjectedJob.h"

#ifndef Tunneling_Analysis_HelpFunctions_h
#include "help/HelpFunctions.h"
#endif


namespace harvest {


    static bool checkHarvestFiles(const std::string& gaussFileEnding){
        bool forward = false;
        bool reverse = false;
        std::string s_cwd;
        size_t pos;
        if ((pos = myOptions::options::getFiles()[0].find_last_of("/")) != std::string::npos )
            s_cwd=myOptions::options::getFiles()[0].substr(0,pos);
        else
            help::getCWD(s_cwd);
        //Check forward files
        std::string harvestPath = s_cwd + "/IRC_projected_forward/";
        //Check for the input files
        int i = 2;
        std::string harvestFilePath = harvestPath + help::to_string(i) + ".com";
        while (help::isFilePathValid(harvestFilePath)) {
            i++;
            harvestFilePath = harvestPath + help::to_string(i) + ".com";
        }
        i--;
        harvestFilePath = harvestPath + help::to_string(i) + gaussFileEnding;
        while (help::isFilePathValid(harvestFilePath)) {
            i--;
            harvestFilePath = harvestPath + help::to_string(i) + gaussFileEnding;
        }
        
        if (i == 1) {
            forward = true;
        }
        
        //Check reverse files
        harvestPath = s_cwd + "/IRC_projected_reverse/";
        i = 1;
        harvestFilePath = harvestPath + help::to_string(i) + ".com";
        while (help::isFilePathValid(harvestFilePath)) {
            i++;
            harvestFilePath = harvestPath + help::to_string(i) + ".com";
        }
        i--;
        harvestFilePath = harvestPath + help::to_string(i) + gaussFileEnding;
        while (help::isFilePathValid(harvestFilePath)) {
            i--;
            harvestFilePath = harvestPath + help::to_string(i) + gaussFileEnding;
        }
        
        if (i == 0) {
            reverse = true;
        }
        return (forward && reverse);
    }
    
    static void harvest(bool verbose, const std::string& gaussFileEnding){
        std::string s_cwd;
        size_t pos;
        if ((pos = myOptions::options::getFiles()[0].find_last_of("/")) != std::string::npos )
            s_cwd=myOptions::options::getFiles()[0].substr(0,pos);
        else
            help::getCWD(s_cwd);
        
        std::string filenameForHarvest = myOptions::options::getprojectedIRCFilename();
        std::fstream IRCProjectedOutFile(filenameForHarvest.c_str(), std::ios::out | std::ios::trunc);
        std::string harvestPath = s_cwd + "/IRC_projected_forward/";
        
        int i = 2; // Case of forward
        std::string harvestFilePath = harvestPath + help::to_string(i) + gaussFileEnding;
        while (help::isFilePathValid(harvestFilePath)) {
            FreqProjectedJob* freqJob = new FreqProjectedJob(harvestFilePath);
            IRCProjectedOutFile << std::setprecision(6) << std::fixed << std::setw(10) << std::left << std::setfill(' ') << freqJob->getJobname() << " " << std::setprecision(6) << std::fixed << std::setw(10) << freqJob->getZPC() << std::endl;
            i++;
            harvestFilePath = harvestPath + help::to_string(i) + gaussFileEnding;
            if (verbose) {
                std::cout << "Writing step " << i << " to file" << std::endl;
                std::cout << std::setprecision(6) << std::fixed << std::setw(10) << std::left << std::setfill(' ') << freqJob->getJobname() << " " << std::setw(10) << freqJob->getZPC() << std::endl;
            }
            delete freqJob;
        }
        
        i = 1; // Case of reverse
        // I need to reverse the values
        harvestPath = s_cwd + "/IRC_projected_reverse/";
        harvestFilePath = harvestPath + help::to_string(i) + gaussFileEnding;
        while (help::isFilePathValid(harvestFilePath)) {
            i++;
            harvestFilePath = harvestPath + help::to_string(i) + gaussFileEnding;
        }
        i--;
        harvestFilePath = harvestPath + help::to_string(i) + gaussFileEnding;
        while (help::isFilePathValid(harvestFilePath)) {
            FreqProjectedJob* freqJob = new FreqProjectedJob(harvestFilePath);
            double jobname;
            help::to_number(jobname, freqJob->getJobname());
            if (jobname == 0) {
                i--;
                harvestFilePath = harvestPath + help::to_string(i) + gaussFileEnding;
                continue;
            }
            IRCProjectedOutFile << std::setprecision(6) << std::fixed << std::setw(10) << std::left << std::setfill(' ') << freqJob->getJobname() << " " << std::setprecision(6) << std::fixed << std::setw(10) << freqJob->getZPC() << std::endl;
            i--;
            harvestFilePath = harvestPath + help::to_string(i) + gaussFileEnding;
            if (verbose) {
                std::cout << "Writing step " << i << " to file" << std::endl;
                std::cout << std::setprecision(6) << std::fixed << std::setw(10) << std::left << std::setfill(' ') << freqJob->getJobname() << " " << std::setw(10) << freqJob->getZPC() << std::endl;
            }
            delete freqJob;
        }
        IRCProjectedOutFile.close();
        return;
    }
}


#endif
