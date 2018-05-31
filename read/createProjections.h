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


#ifndef Tunneling_Analysis_createProjections_h
#define Tunneling_Analysis_createProjections_h
#ifndef Tunneling_Analysis_HelpFunctions_h
#include "../help/HelpFunctions.h"
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <config/Config.h>
#include <array>
#include <boost/filesystem.hpp>
#include "IRCProjectedOut.h"

namespace bFS=boost::filesystem;

namespace createProjections {

enum directions{
	forward,
	reverse
};

const std::array<std::string,2> directionDir= { "/IRC_projected_forward/",
		"/IRC_projected_reverse/" };

std::string getWorkingDirectoryOfFile(const std::string& filename) {
	std::string s_cwd;
	size_t pos;
	if ((pos = filename.find_last_of("/")) != std::string::npos)
		s_cwd = filename.substr(0, pos);
	else
		help::getCWD(s_cwd);

	return s_cwd;
}

bool checkIfProjectionDirExists(const std::string &filename) {
	std::string cwd=getWorkingDirectoryOfFile(filename);
	bFS::path forwardDir(cwd + directionDir[directions::forward]);
	bFS::path reverseDir(cwd + directionDir[directions::reverse]);

	if(bFS::exists(forwardDir) && bFS::exists(reverseDir))
		return true;

	return false;
}


static void createProjections(const std::string& filename, bool sequential, bool execute){
	Config conf(std::string( INSTALL_PREFIX )+ std::string("/etc/config.yml"));
	IRCProjectedOut* aIRCOut = new IRCProjectedOut(filename);
	std::string s_cwd = getWorkingDirectoryOfFile(filename);
	//Determine weather the IRC is forward or reverse
	std::string folder;

	if (aIRCOut->isIRCPathForward())
		folder=directionDir[directions::forward];
	else
		folder=directionDir[directions::reverse];


	std::string mkdict= s_cwd + folder;

	// Create dir for gaussian input files
	mkdir(mkdict.c_str(), 0775);

	//Write the output files to folder

	aIRCOut->getUserInput();

	std::string submitScriptName;
	if (aIRCOut->isIRCPathForward()) {
		submitScriptName = s_cwd + "/start_projections_f.sh";
	}
	else {
		submitScriptName = s_cwd + "/start_projections_r.sh";
	}


	IRCPathVector::const_iterator posInIRC=aIRCOut->getIRCPathVector().begin();
	std::fstream submitScript(submitScriptName.c_str() , std::ios::out | std::ios::trunc);
	submitScript << "#!/bin/sh" << std::endl;
	submitScript << "\n \n" << std::endl;
	while (posInIRC != aIRCOut->getIRCPathVector().end()){
		// Always skip the ZPC for the transition state
		if (posInIRC->RxCoord == 0) {
			posInIRC++;
			continue;
		}
		std::string filename = mkdict + help::to_string(posInIRC->step) + ".com";
		std::fstream file(filename.c_str(), std::ios::out | std::ios::trunc);
		aIRCOut->setCheck(help::to_string(posInIRC->RxCoord));
		aIRCOut->setTitle(help::to_string(posInIRC->RxCoord));
		file << aIRCOut->headerOut();
		aIRCOut->getGeometryAtRxCoord(file, posInIRC->RxCoord,true);
		file << "\n \n";
		if (sequential) {
			submitScript << "echo submitting file: " << filename << ";" << std::endl;
			submitScript << conf.getGaussSubmitCommand(filename) << "; \n" << std::endl;

		}
		else {
			submitScript << conf.getGaussSubmitCommand(filename) << "&" << std::endl;
		}

		file.close();
		posInIRC++;
	}

	//Setting access rights
	std::string command = "chmod 750 " + submitScriptName;

	system(command.c_str());

	std::cout << "Your projections files was written to " << mkdict << std::endl;
	if (execute) {
		system(submitScriptName.c_str());
	}

	submitScript.close();
	delete aIRCOut;

}
}


#endif
