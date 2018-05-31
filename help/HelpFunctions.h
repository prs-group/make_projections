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

#ifndef Tunneling_Analysis_HelpFunctions_h
#define Tunneling_Analysis_HelpFunctions_h
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <stdexcept>

#ifdef __GNUC__
#include <cstdlib>
#endif

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd // stupid MSFT "deprecation" warning
#else
#include <unistd.h>
#endif

// Define Macros to help implement pimpl idom ( example from QT)

/*#define DECLARE_PRIVATE(Class) \
    inline Class##Private* d_func(){return reinterpret_cast<Class##Private*>(d_ptr);} \
    inline const Class##Private* d_func() const{ \
        return reinterpret_cast<Class##Private*>(d_ptr);} \
    friend Class##Private;

#define DECLARE_PUBLIC(Class) \
        inline Class * q_func() {return reinterpret_cast<Class *>(q_ptr);} \
    inline const Class * q_func() const{ \
        return reinterpret_cast<Class *>(q_ptr);} \
    friend Class;

#define D(Class) Class##Private * const d = d_func();
#define Q(Class) Class * const q = q_func();*/


namespace help {

    template <class T>
    inline std::string to_string (const T& t)
    {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }
    
    template <class T>
    inline bool to_number(T& t,const std::string& string){
        std::stringstream iss(string);
        iss.exceptions (std::stringstream::badbit );
        try {
            do {
                iss>>t;
            } while (iss);
        } catch (std::stringstream::failure &e) {
            std::cout << e.what() << std::endl;
            return false;
        } 
        catch (...){
            return false;
        }
        
        return true;
    }
    
    inline void readFileToString(std::fstream &file, std::string& output){
        if (!file.is_open()) {
            return;
        }
        file.seekg(0,std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0,std::ios::beg);
        char* buffer = new char[fileSize];
        file.read(buffer, fileSize);
        file.close();
        output = buffer;
        delete [] buffer;
        return;
    }
    
    inline void getCWD(std::string& s_cwd){
        char buffer[1000];
        char *answer = getcwd(buffer, sizeof(buffer));
        if (answer)
        {
            s_cwd = answer;
        }
    }
    
    inline bool isFilePathValid(const std::string& filepath){
        std::ifstream file(filepath.c_str(),std::ios::in);
        if (!file) {
            file.close();
            return false;
        }
        file.close();
        return true;
    }
    
    inline bool is_empty(std::fstream& pFile)
    {
        return pFile.peek() == std::fstream::traits_type::eof();
    }


    inline void tokenize(const std::string& str, 
                std::vector<std::string> &tokens,
                const std::string& delimiters= " "){
                
        // Skip delimiters at beginning.
        std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
        // Find first "non-delimiter".
        std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

        while (std::string::npos != pos || std::string::npos != lastPos)
        {
            // Found a token, add it to the vector.
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            // Skip delimiters.  Note the "not_of"
            lastPos = str.find_first_not_of(delimiters, pos);
            // Find next "non-delimiter"
            pos = str.find_first_of(delimiters, lastPos);
        }
    }
    
    inline int stringToInt(const std::string s){
        int i;
        std::stringstream iss(s);
        iss >> i;
        if(!iss)
            throw(std::invalid_argument("The string is not convertable to int"));
    
        return i;
    }

    inline double stringToDouble(const std::string s){
        double i;
        std::stringstream iss(s);
        iss >> i;
        if(!iss)
            throw(std::invalid_argument("The string is not convertable to double"));
    
         return i;
    }
    
}



#endif
