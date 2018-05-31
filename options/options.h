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

#ifndef Tunneling_Analysis_options_h
#define Tunneling_Analysis_options_h

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#ifndef Tunneling_Analysis_HelpFunctions_h
#include "help/HelpFunctions.h"
#endif

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>


namespace myOptions {
    template <class T>
    inline std::string to_string (const T& t)
    {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }
    
    class options {
    public:
        static bool setOptions(int, const char**);
        static std::vector<std::string> getFiles();
        static const std::string getFileAt(int index);

        class option_init {
        public:
        	option_init();
        };
    
        // Request methods
        static bool verbose();
        static bool execute();
        static bool help();
        static bool writeIRC();
        static bool harvest();
        static bool sequential();
        static bool isMem();
        static bool isCPUs();
        static bool force();
        static bool highLevelSinglePoints();
        static bool harvestHLSP();
        const static int getMem();
        const static int getnCPUs();
        const static std::string getIRCFilename();
        const static std::string getprojectedIRCFilename();
        const static std::string getHighLevelSinglePointFilename();
    
        static void displayHelp();
        static const int getNumberOfFiles();
        static void initDescription();

    private:
        static boost::program_options::variables_map vm;
        static boost::program_options::options_description visibleOptionDescription;
        static boost::program_options::options_description invisibleOptionDescription;
        static std::vector<std::string> files;
        static int nCPU,mem;
        static std::string ircFile,projecFile,hlspFile;
        static option_init opt_initilizer;

        bool static parseAndPrintError(int argc,const char ** argv,
			boost::program_options::options_description cmdline_options);

};
}

#endif

