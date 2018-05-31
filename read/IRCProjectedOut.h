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


#ifndef Tunneling_Analysis_IRCProjectedOut_h
#define Tunneling_Analysis_IRCProjectedOut_h
#include "IRC_Job.h"
#include <string>
#include <fstream>
#include <iostream>


#define CPUFLAG "cpu"
#define MEMFLAG "mem"
#define MEMLOWWARING 256
#define CPUWARNING 8
#define MEMWARNING 16000

enum errorFlags {
    CPUInputError = 0x01,
    MemInputError = 0x02,
    LowMemoryError = 0x04,
    LowCPUError = 0x10,
    HighCPUError = 0x20,
    HighMemError = 0x40
    };

class IRCProjectedOut: public IRC_Job {
private:
    std::string header;
    std::string mem;
    std::string procs;
    std::string title;
    std::string check;
    
public:
    IRCProjectedOut(const std::string&, const std::string&,const std::string&);
    IRCProjectedOut(const std::string&);
    IRCProjectedOut();
    IRCProjectedOut(const std::string&, const std::string&,const std::string&,const std::string&);
    const std::string headerOut() const;
    
    bool init(std::string);
    bool init(const char*);
    
    friend std::ostream& operator<< (std::ostream &out, IRCProjectedOut &aIRCProjectedOut){
        out << aIRCProjectedOut.header;
        return out;
    }
    const std::string getMem() const{
        return mem;
    }
    const std::string getProcs() const{
        return procs;
    }
    const std::string getTitle() const{
        return title;
    }
    const std::string getCheckFilename() const{
        return check;
    }
    const std::string getHeader() const{
        return header;
    }
    
    void setMem(std::string memory){
        this->mem=memory;
        refreshHeader();
    }
    
    void setProcs(std::string processors){
        this->procs=processors;
        refreshHeader();
    }
    
    void setTitle(std::string theTitle){
        this->title=theTitle;
        refreshHeader();
    }
    
    void setCheck(std::string checkfile){
        this->check=checkfile;
        refreshHeader();
    }
    
    void getUserInput();
    
    void refreshHeader();
    
    
};

class fatalUserErrorInput {
    std::string message;
    
public:
    fatalUserErrorInput(errorFlags flag)
    {
        if (flag & CPUInputError){
            this->message="Sorry you did not enter a valid value for the number of used CPUs";
        }
        else if(flag & MemInputError){
            this->message="Sorry you did not enter a valid value for the memory";
        }
        else{
            this->message="Another error occured during input";
        }
    }
    const std::string& getMessage(){
        return message;
    }
};

#endif
