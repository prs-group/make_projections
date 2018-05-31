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

#include "config.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <regex>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_NCURSES_H
extern "C" {
#include <ncurses.h>
#include <curses.h>
#include <menu.h>
}
#include "GUI/myapp.h"
#endif


#include "read/GaussReading.h"
#include "config/Config.h"
#include "yaml-cpp/yaml.h"
#include "read/createHighLevelSP.h"
#ifdef TEST
#include "Data_Analysis/Plot.h"
#endif

#include "options/options.h"

#ifdef __GNUC__
#include <cstdlib>
#endif

#include <boost/format.hpp>


#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4


using namespace std;
using namespace myOptions;

enum errorTypes {
	fatal=-1,
	warning=1,
	userInput=2,
	logic=3,
	unknown=99
};

bool parseOptions(int argc, const char* argv[]) {
	try {
		if (!options::setOptions(argc, argv))
			return false;
	} catch (exception& e) {
		cerr << e.what() << endl;
		return false;
	}
	if (options::help()) {
		return false;
	}

	return true;
}

#ifdef HAVE_NCURSES_H
void initAndExecuteGUI() {
	static TestApplication* Demo = new TestApplication();
	(*Demo)();
	delete Demo;
}
#endif

void printFiles() {
	for (int j = 0; j < options::getNumberOfFiles(); j++) {
		cout << "You're submitted file was: " << options::getFileAt(j) << endl;
		IRC_Job temp(options::getFileAt(j));
		cout << temp << endl;
	}
}

void writeIRCToFile() {
	//Usually an IRC consists of an forward and reverse calculation. So abort when more than two files are specified (Will implement option to submit only 1 IRC)
	if (options::getNumberOfFiles() != 2) {
		cerr << "You have to submit two IRC files." << endl;
		options::displayHelp();
		exit(EXIT_FAILURE);
	}
	//Get the current working directory
	string s_cwd;
	size_t pos;
	if ((pos = options::getFiles()[0].find_last_of("/")) != std::string::npos)
		s_cwd = options::getFiles()[0].substr(0, pos);
	else
		help::getCWD(s_cwd);

	FBIRC writeIRC(options::getFiles()[0], options::getFiles()[1]);
	//Creating file in current working directory and write IRCs to file( Note: if file already exists it will be overriden)
	string filename = options::getIRCFilename();
	filename = s_cwd + "/" + filename;
	fstream IRC(filename.c_str(), ios::out | ios::trunc);
	IRC << writeIRC;
	IRC.close();
}

bool createHLSPs(const Config& configure) {
	if (options::verbose()) {
		cout << "High energy level input string:\n ";
		cout << configure.getHighLevelInput() << std::endl;
	}
	for (int i = 0; i < 2; i++) {
		std::string filename = options::getFiles()[i];
		if(highLevelSP::checkIfHLSPDirExists(filename))
			return false;
		highLevelSP::createHighLevelSP(filename);
	}
	return true;
}

void harvestHLSPs(const Config& configure) {
	if (options::verbose()) {
		cout << "Your submitted regex was compiled to:\n ";
		cout << configure.getHighLevelRegex() << std::endl;
	}


	std::string s_cwd;
	size_t pos;
	if ((pos = options::getFiles()[0].find_last_of("/")) != std::string::npos)
		s_cwd = options::getFiles()[0].substr(0, pos);
	else
		help::getCWD(s_cwd);

	std::string outFilename = s_cwd + "/IRC_HLSP.txt";
	std::fstream* outFile = new fstream(outFilename,
			std::ios::out | std::ios::trunc);
	for (int i = 0; i < 2; i++) {
		std::string filename = options::getFiles()[i];
		if (!highLevelSP::harvestSP(filename, outFile)) {
			cerr << "Error: The harvest did not work!" << std::endl;
			break;
		}
	}
	std::cout << "The file with the harvested Energies have been written to: "
			<< outFilename << std::endl;
	outFile->close();
	delete outFile;
}

bool makeProjections() {
	//For each of the two files specified
	for (int i = 0; i < options::getNumberOfFiles(); i++){
		if(createProjections::checkIfProjectionDirExists(options::getFileAt(i)))
			return false;

		createProjections::createProjections(options::getFileAt(i),
				options::sequential(), options::execute());
	}

	return true;
}

// The program works as follows:
/* 1. The IRC is always written to file
 * 2. You can change between projected mode and hlsp mode
 */

int main(int argc, const char * argv[])
{
	string configFilename = string(INSTALL_PREFIX) + "/etc/config.yml";
	Config configure(configFilename);

	if(!parseOptions(argc, argv)){
		options::displayHelp();
		return 0;
	}

	if(options::help()){
		options::displayHelp();
		return 0;
	}

#ifdef HAVE_NCURSES_H
	initAndExecuteGUI();
	return 0;
#endif

	try{
		//Action for verbose
		if (options::verbose())
			printFiles();

		writeIRCToFile();

		// Action for generating high Level single points or else projected IRC mode
		if (options::highLevelSinglePoints()){
			if(!createHLSPs(configure))
				harvestHLSPs(configure);

			return 0;
		}

		//If make Projections fails (because the projections were already created): Harvest
		if(!makeProjections()){
			harvest::harvest(options::verbose(), configure.getGaussFileEnding());
			return 0;
		}
	}
	catch(fatalGaussianError& e)
	{
		cerr << e.what() << endl;
		return errorTypes::fatal;
	}
	catch(warningGaussianError& e){
		cerr << e.what() << endl;
		return errorTypes::userInput;
	}
	catch(unsigned char& e){
		cerr << e << endl;
		return errorTypes::unknown;
	}
	catch(boost::io::bad_format_string &e){
		cerr << "Check your configuration: " << e.what() << endl;
		return errorTypes::userInput;
	}
	catch(std::regex_error &e){
		cerr << "A " << e.what() << " occured with code: " << e.code() << endl;
		return errorTypes::logic;
	}
	catch(...){
		cerr << "An unkown error occured!" << endl;
		return errorTypes::fatal;
	}

	return 0;

}

