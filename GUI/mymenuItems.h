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


#ifndef Tunneling_Analysis_myactions_h
#define Tunneling_Analysis_myactions_h

#include "config.h"

#include <stdlib.h>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

#include <cursesm.h>
#include <cursesapp.h>
#include <cursesf.h>


#include "options/options.h"
#include "read/harvest.h"
#include "config/Config.h"


class writeIRC {
    
public:
    void run();
};

class showIRC {
    
public:
    void run();
};

class harvestAction {
    
public:
    void run();
};

class createAction{
  
public:
    void run();
};

class IRCAction : public NCursesMenuItem {
    
public:
    IRCAction(const char* name) : NCursesMenuItem(name)
    {}
    
     virtual ~IRCAction() {}
    
    bool action(){
        writeIRC IRC;
        IRC.run();
        return FALSE;
    }
};

class showIRCFile : public NCursesMenuItem {
    
public:
    showIRCFile(const char* name): NCursesMenuItem(name)
    {}
    
    virtual ~showIRCFile(){}
    
    bool action(){
        showIRC IRC;
        IRC.run();
        return FALSE;
    }
};

class harvestProjections : public NCursesMenuItem {
    
public:
    harvestProjections(const char* name) : NCursesMenuItem(name)
    {
        Config conf;
        if (!harvest::checkHarvestFiles(conf.getGaussFileEnding())) {
            options_off(O_SELECTABLE);
        }
    }
    
    virtual ~harvestProjections(){}
    
    bool action(){
        harvestAction harvest;
        harvest.run();
        return FALSE;
    }
};

class createTheProjection : public NCursesMenuItem{
    
public:
    createTheProjection(const char* name) : NCursesMenuItem(name){
    }
    
    virtual ~createTheProjection(){}

    bool action(){
        createAction create;
        create.run();
        return FALSE;
    }
};

class PassiveItem : public NCursesMenuItem
{
public:
    PassiveItem(const char* text) : NCursesMenuItem(text) {
        options_off(O_SELECTABLE);
    }
};

class QuitItem : public NCursesMenuItem
{
public:
    QuitItem() : NCursesMenuItem("Quit") {
    }
    
    bool action() {
        return TRUE;
    }
};

class fileBrowser {
    std::vector<boost::filesystem::path> getDirectoryContent(const std::string& dir);
    void selectItem(NCursesPanel* panel,int i);
    void deselectItem(NCursesPanel* panel, int i);
    void startUserLoop(NCursesPanel* panel);
    bool isSelected(const boost::filesystem::path &p) const;
    void updateDisplayedFiles();
    void updateDisplay(NCursesPanel* panel);
    bool checkFiles(); //Checks every file and displays error
    int currentChoice; // Line of current selection (start at 1)
    int numberOfFiles;
    int topLimit,height; // These numbers contains the current display limits 
    const std::string selected="*", choice="-";
    bool scroll=false; // Scrolling enabled (is set automatically)
    boost::filesystem::path dirname;
    std::vector<boost::filesystem::path> filelist;
    std::vector<boost::filesystem::path> selectedFiles; // Contains all files in dirname
    std::vector<boost::filesystem::path> displayedFiles; 
    boost::filesystem::path cur_dir;
    // Contains only files that are displayed
    
    public:
    void run();
};

class selectFiles : public NCursesMenuItem
{
    public:
        selectFiles(const char* text) : NCursesMenuItem(text) {
        
        }
        
        bool action(){
            fileBrowser f;
            f.run();
            return FALSE;
        }
    private:
        
};

#endif
