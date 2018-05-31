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


#include <iostream>
#include <string>
#include "IRCProjectedOut.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "options/options.h"
#include "../help/HelpFunctions.h"
#include <stdio.h>
using namespace std;


IRCProjectedOut::IRCProjectedOut(const string& filename):IRC_Job(filename),mem("256"),procs("1"),title("Default"),check("default")
{
    string initString="%mem="+mem+"mb \n"+
    "%NProcShared="+ procs + "\n"+
    "#p "+ getCalculationMethod() + " freq=(noraman,projected) Int=UltraFine \n"+
    "\n"+
    title+"\n"+
    "\n"+
    help::to_string(this->getChargeAndMultiplicity().charge)+" "+ help::to_string(this->getChargeAndMultiplicity().multiplizity)+ "\n";
    this->header=initString;
}

IRCProjectedOut::IRCProjectedOut(const string& filename,const string& procs="1",const string& mem="256"):IRC_Job(filename),mem(mem),procs(procs),title("Default"),check("default")
{
    string initString="%mem="+mem+"mb \n"+
    "%NProcShared="+ procs + "\n"+
    "#p "+ getCalculationMethod() + " freq=(noraman,projected) Int=UltraFine \n"+
    "\n"+
    title+"\n"+
    "\n"+
    help::to_string(this->getChargeAndMultiplicity().charge)+" "+ help::to_string(this->getChargeAndMultiplicity().multiplizity)+ "\n";
    header=initString;
}

IRCProjectedOut::IRCProjectedOut(const string& filename,const string& procs="1",const string& mem="256",const string &outFilename=" "):IRC_Job(filename),mem(mem),procs(procs),title(outFilename),check(outFilename)
{
    string initString="%mem="+mem+"mb \n"+
    "%NProcShared="+ procs + "\n"+
    "#p "+ getCalculationMethod() + " freq=(noraman,projected) Int=UltraFine \n"+
    "\n"+
    title+"\n"+
    "\n"+
    help::to_string(this->getChargeAndMultiplicity().charge)+" "+ help::to_string(this->getChargeAndMultiplicity().multiplizity)+ "\n";
    header=initString;
}

IRCProjectedOut::IRCProjectedOut():header(" ") ,mem("256"),procs("1"),title("Default"),check("default"){
    
}

bool IRCProjectedOut::init(string filename){
 
    if (!this->IRC_Job::init(filename)) {
        return false;
    }
    this->procs = "1";
    this->mem = "256";
    this->check = "Check";
    this->title = "Default";
    
    this->refreshHeader();
    return true;
}

bool IRCProjectedOut::init(const char* filename){

    if (!this->IRC_Job::init(filename)) {
        return false;
    }
    
    this->procs = "1";
    this->mem = "256";
    this->check = "Check";
    this->title = "Default";
    
    this->refreshHeader();
    return true;
}

const  string IRCProjectedOut::headerOut() const{
    return header;
}

void IRCProjectedOut::refreshHeader(){
    string initString="%NProcShared="+ procs + "\n"+
    "%mem="+mem+"mb \n"+
    "#p "+ getCalculationMethod() + " freq=(noraman,projected) Int=UltraFine \n"+
    "\n"+
    title+"\n"+
    "\n"+
    help::to_string(this->getChargeAndMultiplicity().charge)+" "+ help::to_string(this->getChargeAndMultiplicity().multiplizity)+ "\n";
    
    this->header=initString;
}

void IRCProjectedOut::getUserInput(){
    if (myOptions::options::isMem() && myOptions::options::isMem()) {
        int imem=myOptions::options::getMem();
        int inCPU=myOptions::options::getnCPUs();
        if (inCPU < 1) {
            cout << "You have to enter a number less than 1." << endl;
            exit(1);
        }
        
        if (imem < MEMLOWWARING){
            cout << "You have entered too less memory. 256 mb will be used instead" << endl;
            mem=256;
        }
        this->mem=help::to_string(imem);
        this->procs=help::to_string(inCPU);
    } else if(!isatty(fileno(stdin))){
        mem="256";
        procs="1";
        
        }else{
        try {
            cout << "Configuration for file " << this->getFilename() << endl;
            string cpuInput = "Please enter the number of CPUs you will use for the projected calculation: ";
            cout << setw(static_cast<int>(cpuInput.length())) << setfill('-') << "" << endl;
            cin.sync();cin.clear();
            int nprocs;
            cout << cpuInput;
            cin >> nprocs;

        
            if (cin.fail()) {
                throw fatalUserErrorInput(CPUInputError);
            }
        
            while (nprocs < 1) {
                cout << "You have to enter a number less than 1. Try again\n";
                cin.sync();cin.clear();
                cin >> nprocs;
                cout << endl;
                if (cin.bad()) {
                    throw fatalUserErrorInput(CPUInputError);
                }
            }
        
            if (nprocs > CPUWARNING) {
                cout << "You enterted more than " << CPUWARNING << " CPUs. Are you sure you want to continue (Y/N)? ";
                cin.sync(); cin.clear();
                char c;
                do {
                    cin.get(c);
                    if (cin.fail())
                        throw runtime_error("Error during Input");
                } while (c != 'Y' && c != 'y' && c != 'N' && c != 'n');
            
                if (c == 'N' || c == 'n') {
                    exit(EXIT_FAILURE);
                }
            
                cout << endl;
            }
            this->procs=help::to_string(nprocs);
        
            this->refreshHeader();
        
        } catch (fatalUserErrorInput &e) {
            throw fatalGaussianError(e.getMessage().c_str());
        } catch(exception &e) {
            throw fatalGaussianError(e.what());
        } catch(...){
            throw fatalGaussianError();
        }
    
        try{
            string memInput = "Please enter the memory (MB) you will use during your calculation: ";
            cout << setw(static_cast<int>(memInput.length())) << setfill('-') << "" << endl;
            cin.sync();cin.clear();
            int memory;
            cout << "Please enter the memory (MB) you will use during your calculation: ";
            cin >> memory;
        
            if (cin.fail()) {
                throw fatalUserErrorInput(MemInputError);
            }
        
            if (memory > MEMWARNING) {
                cout << "You enterted more than "<< MEMWARNING << " MB memory. Are you sure you want to continue (Y/N) ";
                cin.sync(); cin.clear();
                char c;
                do {
                    cin.get(c);
                } while (c != 'Y' && c != 'y' && c != 'N' && c != 'n');
                cin.sync(); cin.clear();
                if (c=='N' || c=='n') {
                    exit(EXIT_FAILURE);
                }
                cout << endl;
            }
            else if(memory < MEMLOWWARING)
            {
                cout << "Warning : You entered too less memory"  << endl;
                cout << "The standard value of 256 mb used instead." << endl;
            }
        
            this->mem=help::to_string(memory);
            this->refreshHeader();
        } catch (fatalUserErrorInput e) {
            throw fatalGaussianError(e.getMessage().c_str());
        }catch(...){
            throw fatalGaussianError();
        }
    }
}

