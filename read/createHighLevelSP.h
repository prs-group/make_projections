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




#ifndef make_projection_createHighLevelSP_h
#define make_projection_createHighLevelSP_h

#include "options/options.h"
#include "IRCProjectedOut.h"
#include <string>
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include <sys/stat.h>
#include <config/Config.h>
#include "IRCProjectedOut.h"
#include "help/HelpFunctions.h"
#include <boost/format.hpp>
#include <boost/regex.hpp>
#include <sys/stat.h>

namespace bFS=boost::filesystem;

namespace highLevelSP {
enum directions{
	forward,
	reverse
};

const std::array<std::string,2> directionDir= { "/IRC_SP_forward/",
		"/IRC_SP_reverse/" };

std::string getWorkingDirectoryOfFile(const std::string& filename) {
	std::string s_cwd;
	size_t pos;
	if ((pos = filename.find_last_of("/")) != std::string::npos)
		s_cwd = filename.substr(0, pos);
	else
		help::getCWD(s_cwd);

	return s_cwd;
}

bool checkIfHLSPDirExists(const std::string &filename) {
	std::string cwd=getWorkingDirectoryOfFile(filename);
	bFS::path forwardDir(cwd + directionDir[directions::forward]);
	bFS::path reverseDir(cwd + directionDir[directions::reverse]);

	if(bFS::exists(forwardDir) && bFS::exists(reverseDir))
		return true;


	return false;
}

void createHLSPInputFiles(const std::string& outDirectory,
		const boost::format& inputFormat, IRCProjectedOut* theIRC) {
	int mem = myOptions::options::getMem(), cpus =
			myOptions::options::getnCPUs(), mul =
					theIRC->getChargeAndMultiplicity().multiplizity, charge =
							theIRC->getChargeAndMultiplicity().charge;
	std::string basisSet = theIRC->getCalculationMethod();
	basisSet = basisSet.substr(basisSet.find('/') + 1, std::string::npos);
	IRCPathVector::const_iterator posInIRC = theIRC->getIRCPathVector().begin();
	while (posInIRC != theIRC->getIRCPathVector().end()) {
		double rxCoord = posInIRC->RxCoord;
		boost::format frmt(inputFormat);
		std::string filename = outDirectory + help::to_string(posInIRC->step)
		+ ".com";
		std::string geom = theIRC->getGeometryAtRxCoord(rxCoord, true);
		std::string title = help::to_string(posInIRC->RxCoord);
		std::stringstream outString;
		outString << frmt % title % geom % mem % cpus % charge % mul
		% basisSet << std::endl;
		std::fstream outFile(filename.c_str(), std::ios::out | std::ios::trunc);
		outFile << outString.str();
		outFile << std::endl;
		outFile << std::endl;
		outFile.close();
		posInIRC++;
	}
}

void createSubmitScript(const std::string& submitScriptName) {
	std::fstream submitScript(submitScriptName.c_str(),
			std::ios::out | std::ios::trunc);
	submitScript << "#! /bin/sh" << std::endl;
	submitScript << "\n \n" << std::endl;
	std::string command = "chmod 750 " + submitScriptName;
	//Setting access rights
	system(command.c_str());
	submitScript.close();
}

bool createHighLevelSP(std::string &filename){
	Config conf(std::string( INSTALL_PREFIX )+ std::string("/etc/config.yml"));
	IRCProjectedOut* aIRCOut = new IRCProjectedOut(filename);
	std::string s_cwd;
	s_cwd=getWorkingDirectoryOfFile(filename);
	//Determine weather the IRC is forward or reverse
	std::string direction;
	if (aIRCOut->isIRCPathForward()) {
		direction=directionDir[directions::forward];
	}
	else {
		direction=directionDir[directions::reverse];
	}
	std::string mkdict= s_cwd + direction;
	// Needed variables for format string
	const boost::format fmtr(conf.getHighLevelInput());
	// Create dir for SP input files
	mkdir(mkdict.c_str(), 0775);
	createHLSPInputFiles(mkdict, fmtr, aIRCOut);

	std::string submitScriptName;
	if (aIRCOut->isIRCPathForward()) {
		submitScriptName = s_cwd + "/start_SP_f.sh";
	}
	else {
		submitScriptName = s_cwd + "/start_SP_r.sh";
	}

	createSubmitScript(submitScriptName);
	std::cout << "Your single point input files were written to " << mkdict << std::endl;

	// Cleanup
	delete aIRCOut;

	return true;
}

bool harvestSP(std::string &ircFile, std::fstream* outF){
	if(!outF)
		return false;
	IRC_Job *job=new IRC_Job(ircFile);
	Config c;
	std::string s_cwd, direction,filename,outFilename;
	boost::regex energyR(c.getHighLevelRegex());
	size_t pos;
	const std::string folder("/SP_IRC_");
	if ((pos = ircFile.find_last_of("/")) != std::string::npos )
		s_cwd=ircFile.substr(0,pos);
	else
		help::getCWD(s_cwd);
	//Determine weather the IRC is forward or reverse
	if (job->isIRCPathForward()) {
		direction="forward/";
	}
	else {
		direction="reverse/";
	}

	for (auto &step : job->getIRCPathVector()) {
		auto fileExists=[](std::string name) -> bool{
			struct stat buffer;
			return (stat (name.c_str(), &buffer) == 0);
		};

		double rxCoord=step.RxCoord;
		int s=step.step;

		filename=s_cwd + folder + direction + help::to_string<int>(s) + ".out";


		if(!fileExists(filename)){
			std::cout << "Warning: File " << filename << " does not exists! Skipping..." << std::endl;
			continue;
		}

		std::fstream hlspFile(filename.c_str(), std::ios::in);

		std::string hlspFileContent;
		help::readFileToString(hlspFile, hlspFileContent);
		boost::smatch res;

		if(boost::regex_search(hlspFileContent, res, energyR)){
			std::string energy;
			energy=res[1];
			*outF << std::setw(20) << std::left << rxCoord << std::setfill(' ') << energy << std::endl;
		} else
			std::cout << "Warning: You're regex do not seem to work" << std::endl;


		hlspFile.close();


		//std::cout << filename << std::endl;
	}


	delete job;
	return true;
}
}


#endif
