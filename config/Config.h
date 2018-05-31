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


#ifndef Tunneling_Analysis_Config_h
#define Tunneling_Analysis_Config_h

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "config.h"

#include "yaml-cpp/yaml.h"
#include "help/HelpFunctions.h"
#ifdef __GNUC__
#include <cstdlib>
#endif

using pseMap=std::map<int,std::string>;


/*namespace YAML {
    template<>
    static bool decode(const Node& node, pseSet &rhs){
        if(!node.isMap())
            return false;
        
        rhs.insert(std::pair<node.key,)
    }
}*/



class Config {
    std::string gaussSubmitCommand;
    std::string outFileEnding;
    std::string highLevelInput="%1% %2% %3% %4% %5% %6% %7%";
    const std::string pseString="pse: \n 1: H\n 2: He\n 3: Li\n 4: Be\n 5: B\n 6: C\n 7: N\n 8: O\n 9: F \n 10: Ne\n 11: Na\n 12: Mg\n 13: Al\n 14: Si\n 15: P\n 16: S\n 17: Cl\n 18: Ar\n 19: K\n 20: Ca\n 21: Sc\n 22: Ti\n 23: V\n 24: Cr\n 25: Mn\n 26: Fe\n 27: Co\n 28: Ni\n 29: Cu\n 30: Zn\n 31: Ga\n 32: Ge\n 33: As\n 34: Se\n 35: Br\n 36: Kr\n 37: Rb\n 38: Sr\n 39: Y\n 40: Zr\n 41: Nb\n 42: Mo\n 43: Tc\n 44: Ru\n 45: Rh\n 46: Pd\n 47: Ag\n 48: Cd\n 49: In\n 50: Sn\n 51: Sb\n 52: Te\n 53: I\n 54: Xe\n 55: Cs\n 56: Ba\n 57: La\n 58: Ce\n 59: Pr\n 60: Nd\n 61: Pm\n 62: Sm\n 63: Eu\n 64: Gd\n 65: Tb\n 66: Dy\n 67: Ho\n 68: Er\n 69: Tm\n 70: Yb\n 71: Lu\n 72: Hf\n 73: Ta\n 74: W\n 75: Re\n 76: Os\n 77: Ir\n 78: Pt\n 79: Au\n 80: Hg\n 81: Tl\n 82: Pb\n 83: Bi\n 84: Po\n 85: At\n 86: Rn\n 87: Fr\n 88: Ra\n 89: Ac\n 90: Th\n 91: Pa\n 92: U\n 93: Np\n 94: Pu\n 95: Am\n 96: Cm\n 97: Bk\n 98: Cf\n 99: Es\n 100: Fm\n 101: Md\n 102: No\n 103: Lr\n 104: Rf\n 105: Db\n 106: Sg\n 107: Ns\n 108: Bh\n 109: Hs\n 110: Mt\n 111: Ds\n 112: Rg\n 113: Cn\n 115: Fl\n 116: Lv\n";
    std::string hlHarvestCommand="Put string here";
    pseMap pse;
    const std::string configFilename;
    bool firstTime;

    void readConfigFile(){

        YAML::Node doc= YAML::LoadFile(this->configFilename);
        this->gaussSubmitCommand=doc["gaussSubmitCommand"].as<std::string>();
        this->outFileEnding=doc["gaussOutFileEnding"].as<std::string>();
        this->highLevelInput=doc["highLevelInput"].as<std::string>();
        this->hlHarvestCommand=doc["highLevelHarvestRegex"].as<std::string>();
        for (auto i = doc["pse"].begin(); i != doc["pse"].end(); i++) {
            pse.insert(std::pair<int,std::string>(i->first.as<int>(),i->second.as<std::string>()));
        }
 
        // Fix high level String: remove space after newline

        for( auto c=highLevelInput.begin(); c != highLevelInput.end() -1; c++){
            if (*c == '\n' && *(c+1) == ' ')
                highLevelInput.erase(c+1);
        }
    }
    
    void configure(){
        std::fstream iFile(this->configFilename.c_str(), std::ios::in);
        //Check if file already exists if not the program was not configured
        this->firstTime = false;
        if (!iFile.is_open()) {
            this->firstTime = true;
            //Create new file
            iFile.open(this->configFilename.c_str(), std::ios::out | std::ios::trunc);
        }
        
        if (this->firstTime) {
            char answer;
            std::cout << "You're using this program for the first time. You need to answer some questions in order to let the program work" << std::endl;
                do {
                    std::cout << "What is your submit command for Gaussian?  (Enter %s where your file should stand)" << std::endl;
                    std::getline(std::cin, this->gaussSubmitCommand);
                    std::cout << std::endl;
                    std::cout << "You entered " << this->gaussSubmitCommand << " as your Gaussian submit command. Is this right? (Y/N) ";
                    do {
                        std::cin.get(answer);
                    } while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
                    std::cout << std::endl;
                } while(answer != 'y' && answer != 'Y');
            
            do {
                std::cout << "What is your file ending for Gaussian output files?  ";
                std::cin >> this->outFileEnding;
                std::cout << std::endl;
                std::cout << "You entered " << this->outFileEnding << " as your Gaussian file ending. Is this right? (Y/N) ";
                do {
                    std::cin.get(answer);
                } while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
                std::cout << std::endl;
            } while(answer != 'y' && answer != 'Y');
            
            std::cout << "Warning: in order to work with high level single points"
            << " you have to edit the config file manual!" << std::endl;
            
            //Emitting to yaml file
            YAML::Emitter outYAML;
            outYAML << YAML::BeginMap;
            outYAML << YAML::Key << "gaussSubmitCommand";
            outYAML << YAML::Value << this->gaussSubmitCommand;
            outYAML << YAML::Key << "gaussOutFileEnding";
            outYAML << YAML::Value << this->outFileEnding;
            outYAML << YAML::Newline;
            outYAML << YAML::Comment("The input string is formatted using formatted string");
            outYAML << YAML::Newline;
            outYAML << YAML::Comment("%1%: Title");
            outYAML << YAML::Newline;
            outYAML << YAML::Comment("%2%: Geometry");
            outYAML << YAML::Newline;
            outYAML << YAML::Comment("%3%: memory");
            outYAML << YAML::Newline;
            outYAML << YAML::Comment("%4%: number of CPUs");
            outYAML << YAML::Newline;
            outYAML << YAML::Comment("%5%: Charge");
            outYAML << YAML::Newline;
            outYAML << YAML::Comment("%6%: Multiplicity");
            outYAML << YAML::Newline;
            outYAML << YAML::Comment("Basis Set: %7%");
            outYAML << YAML::Newline;
            outYAML << YAML::Key << "highLevelInput";
            outYAML << YAML::Value << this->highLevelInput;
            outYAML << YAML::Key << "highLevelHarvestRegex";
            outYAML << YAML::Value << this->hlHarvestCommand;
            outYAML << YAML::Comment("Regex must enclose energy with ()");
            outYAML << YAML::Newline;
            outYAML << YAML::EndMap;
            
            iFile << outYAML.c_str();
            iFile << pseString.c_str();
            std::cout << "A new config file is created!" << std::endl;
            }
            iFile.close();
        }
    
public:
    Config(std::string configFilename=std::string(INSTALL_PREFIX) + "/etc/config.yml"):configFilename(configFilename){
        //Look if file exists
        try {
            configure();
            readConfigFile();
        } catch (YAML::ParserException &e) {
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        } catch(std::runtime_error &e){
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        } catch(...){
            std::cerr << "An internal error occurred during configuration" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    
    bool isFirstTime(){
        return this->firstTime;
    }
    
    const std::string getGaussSubmitCommand(std::string file) const{
        char buff[1000];
        sprintf(buff, this->gaussSubmitCommand.c_str(), file.c_str());
        return std::string(buff);
    }
    
    const std::string getGaussFileEnding() const{
        return this->outFileEnding;
    }
    const std::string getHighLevelInput() const{
        return this->highLevelInput;
    }
    
    const pseMap& getPSE() const{
        return this->pse;
    }
    
    const std::string getHighLevelRegex() const{
        return this->hlHarvestCommand;
    }

};


#endif
