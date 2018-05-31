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

#include "options.h"
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <iostream>

using namespace myOptions;

namespace po = boost::program_options;


// Defines for static variables
po::variables_map options::vm;

po::options_description options::visibleOptionDescription(
		"Usage: [options] IRC_forward IRC_reverse");
po::options_description options::invisibleOptionDescription("Invisible Options");
std::vector<std::string> options::files;
int options::nCPU;
int options::mem;
std::string options::ircFile;
std::string options::projecFile;
std::string options::hlspFile;
options::option_init opt_initilizer;

options::option_init::option_init(){
	initDescription();
}

bool options::parseAndPrintError(int argc, const char ** argv, po::options_description cmdline_options){
    po::command_line_parser parser(argc,argv);
    po::positional_options_description positionalArguments;

    positionalArguments.add("IRCFiles", 2);
    try {
    	po::store(parser.options(cmdline_options).positional(positionalArguments).run(),
    		vm);
    	po::notify(vm);
    } catch (po::too_many_positional_options_error &e){
    	std::cerr << "Too many positional arguments: " << e.what() << std::endl;
    	return false;
    } catch (po::error_with_option_name &e){
    	std::cerr << "An error with the option name: " << e.what() << std::endl;
    	return false;
    } catch (po::multiple_values &e) {
    	std::cerr << "You can not specify multiple values: " << e.what() << std::endl;
    	return false;
    } catch (po::multiple_occurrences &e){
    	std::cerr << "Multiple occurrence of an option: " << e.what() << std::endl;
    } catch (po::invalid_option_value &e) {
    	std::cerr << "Invalid value for option: " << e.what() << std::endl;
    	return false;
    }  catch (po::unknown_option &e){
    	std::cerr << "Unknown option: " << e.what() << std::endl;
    	return false;
    } catch (po::error &e){
    	std::cerr << "Unhandled Error Occurred: " << e.what() << std::endl;
    	return false;
    } catch (...){
    	return false;
    }

    return true;
}

void options::initDescription() {
	visibleOptionDescription.add_options()("help,h", "Display this help")(
			"verbose,v", "verbose mode")
			("execute,e",
			"execute generated submit scripts")
			("sequential,s",
			"submit commands sequential")
			("writeIRC,w",
			po::value<std::string>(&ircFile)->default_value("IRC.txt"),
			"Specify filename for electronic IRC.")(
			"harvest,g",
			po::value<std::string>(&projecFile)->default_value(
					"IRC_projected.txt"), "Write the zero point corrected energies to file.")
			("mem,m", po::value<int>(&mem)->default_value(256),
			"Memory for each projected computations")
			("nproc,n",	po::value<int>(&nCPU)->default_value(1),
			"Number of CPUs for each projected computation")
			("force,f",	"try to read file even if not terminated successfully")
			("highLevelSinglePoints,l", "generate high level single points")
			("hlspHarvest,j", po::value<std::string>(&hlspFile)->default_value("IRC_hlsp.txt"),
			"harvest high level single points");

    invisibleOptionDescription.add_options()
    		("IRCFiles", po::value< std::vector<std::string> >(&files),
    				"Forward and Reverse IRC")
			;
}

bool options::setOptions(int argc, const char** argv){
	vm.clear();
    po::options_description cmdline_options;
    cmdline_options.add(visibleOptionDescription).add(invisibleOptionDescription);
    return parseAndPrintError(argc,argv,cmdline_options);
}

bool options::verbose(){ return vm.count("verbose") > 0 ; }
bool options::execute(){ return vm.count("execute") > 0; }
bool options::help(){ return vm.count("help") > 0; }
bool options::writeIRC(){ return vm.count("writeIRC") > 0; }
bool options::harvest(){ return vm.count("harvest") > 0 ; }
bool options::sequential(){ return vm.count("sequential") > 0; }
bool options::isCPUs() { return vm.count("mem") > 0; }
bool options::isMem() { return vm.count("nproc") > 0; }
bool options::force() { return vm.count("force") > 0; }
bool options::harvestHLSP() { return vm.count("hlspHarvest") > 0; }
bool options::highLevelSinglePoints() { return vm.count("highLevelSinglePoints") > 0; };
const int options::getnCPUs() { return nCPU; }
const int options::getMem() { return mem; }
const std::string options::getIRCFilename() {
    return ircFile;
}

const std::string options::getprojectedIRCFilename() {
    return projecFile;
}


void options::displayHelp(){
    std::cout << visibleOptionDescription << std::endl;
}

std::vector<std::string> options::getFiles(){
    return files;
}
const std::string options::getFileAt(int index){
    if (index > files.size()) {
    	return "";
    } else
        return files[index];
}


const int options::getNumberOfFiles(){
    return files.size();
}

const std::string myOptions::options::getHighLevelSinglePointFilename() {
	return hlspFile;
}
