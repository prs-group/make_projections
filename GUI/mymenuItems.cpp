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


#include "mymenuItems.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/filesystem.hpp>
#include <chrono>
#include <thread>
#include "../read/FBIRC.h"
#include "../help/HelpFunctions.h"
#include "../config/Config.h"
#include "../read/harvest.h"
#include "../read/createProjections.h"


using namespace std;
using namespace boost::filesystem;


void writeIRC::run(){
    string success = "The IRC has written to file succesfully. Press <enter> to continue";
    string error = "You have to submit two IRC files. Press <enter> to continue";
    NCursesPanel* myStd = new NCursesPanel();
    myStd->boldframe("write IRC");
    NCursesPanel* output = new NCursesPanel(myStd->height()-4,myStd->width()-6, 2,2);
    output->box();
    myStd->refresh();
    output->refresh();
    if (myOptions::options::getNumberOfFiles() != 2) {
        output->printw(output->height()/2, (output->width()-error.length())/2, "%s", error.c_str());
        myStd->refresh();
        output->CUR_getch();
        delete output;
        delete myStd;
        return;
    }
    //Get the current working directory
    std::string s_cwd;
    help::getCWD(s_cwd);
    try {
        FBIRC writeIRC(myOptions::options::getFiles()[0], myOptions::options::getFiles()[1]);
        //Creating file in current working directory and write IRCs to file( Note: if file already exists it will be overriden)
        std::string filename = s_cwd + "/IRC.txt";
        std::fstream IRC(filename.c_str(), std::ios::out | std::ios::trunc);
        IRC << writeIRC;
        IRC.close();
    } catch (fatalGaussianError* e) {
        string error(e->what());
        output->printw(output->height()/2, (output->width()-error.length())/2, "%s", error.c_str());
    } catch(warningGaussianError* e){
        string error(e->what());
        error += ". Press <enter> to continue";
        output->printw(output->height()/2, (output->width()-error.length())/2, "%s", error.c_str());
    } catch(...){
        string error("A fatal error occured. Press <enter> to continue");
        output->printw(output->height()/2, (output->width()-error.length())/2, "%s", error.c_str());
    }
    
    myStd->refresh();
    output->printw(output->height()/2, (output->width()-success.length())/2, "%s", success.c_str());
    output->CUR_getch();
    output->refresh();
    output->clear();
    myStd->clear();
    
    delete output;
    delete myStd;
}

void showIRC::run(){
    NCursesPanel* myStd = new NCursesPanel();
    NCursesPanel* output = new NCursesPanel(myStd->height()-4, myStd->width()-6, 2, 2);
    
    string continueMessage = "Continue with <enter>";
    myStd->refresh();
    output->refresh();
    myStd->boldframe("Show IRC");
    output->box();
    
    try {
        for (int j = 0; j < myOptions::options::getNumberOfFiles(); j++) {
            output->clear();
            IRC_Job temp(myOptions::options::getFileAt(j));
            char *line = new char[1000];
            stringstream* iss = new stringstream;
            *iss << temp;
            //Count number of lines in issstream
            int nrows=1;
            while (iss->getline(line, 1000)) {
                nrows++;
            }
            NCursesFramedPad FP(*output, nrows,output->width()-2);
            delete iss;
            iss = new stringstream;
            *iss << temp;
            for (int row = 2; iss->getline(line, 1000); row++) {
                FP.printw(row-1,10, "%s",line);
            }
            
            FP();
     
            /*int numberOfSites = 1;
            for (int row = 2; iss->getline(line, 1000); row++){
                if (row > output->height()-2) {
                    numberOfSites++;
                    row = 2;
                }
                
            }
            delete [] line;
            delete iss;
            iss = new stringstream;
            *iss << temp;
            stringstream siteMessage;
            siteMessage << "1/" << numberOfSites << endl;
            output->printw(0,output->width()-6,"%s", siteMessage.str().c_str());
            output->refresh();
            line = new char[1000];
            if (iss->eof()) {
                output->printw(2,2,"%s", "Test");
            }
            int currentSite = 1;
            for (int row = 2; iss->getline(line, 1000); row++) {
                
                if (row > output->height()-2) {
                    output->printw(output->height()-1, output->width()-static_cast<int>(continueMessage.length()),"%s", continueMessage.c_str());
                    output->refresh();
                    output->CUR_getch();
                    output->clear();
                    row = 2;
                    currentSite++;
                    siteMessage << currentSite << "/" << numberOfSites << endl;
                    output->printw(0, output->width()-6, "%s", siteMessage.str().c_str());
                    output->refresh();
                }
                
                output->printw(row-1, 10, "%s", line);
            }
            output->refresh();
            delete [] line;
            delete iss;*/
        }
    } catch (fatalGaussianError* e) {
        string error(e->what());
        output->printw(output->height()/2, (output->width()-error.length())/2, "%s", error.c_str());
    } catch(warningGaussianError* e){
        string error(e->what());
        error += ". Press <enter> to continue";
        output->printw(output->height()/2, (output->width()-error.length())/2, "%s", error.c_str());
    } catch(...){
        string error("A fatal error occured. Press <enter> to continue");
        output->printw(output->height()/2, (output->width()-error.length())/2, "%s", error.c_str());
    }
    output->CUR_getch();
    output->clear();
    myStd->clear();
    delete output;
    delete myStd;
}

void harvestAction::run(){
    NCursesPanel* myStd = new NCursesPanel();
    NCursesPanel* output = new NCursesPanel(myStd->height()-4, myStd->width()-6, 2, 2);
    string success = "The projections files have been harvested successfully! Continue with <enter>";
    string failure = "There was an error during harvesting! Continue with <enter>";
    myStd->refresh();
    output->refresh();
    myStd->boldframe("Harvest Projections");
    output->box();
    Config conf;
    if (harvest::checkHarvestFiles(conf.getGaussFileEnding())) {
        harvest::harvest(false, conf.getGaussFileEnding());
        output->printw(output->height()/2, (output->width()-static_cast<int>(success.length()))/2, "%s", success.c_str());
    }
    else
        output->printw(output->height()/2, (output->width()-static_cast<int>(failure.length()))/2, "%s", failure.c_str());

    output->refresh();
    output->CUR_getch();
    
    delete output;
    delete myStd;
}

void createAction::run(){
    Config configure;
    NCursesPanel* myStd = new NCursesPanel();
    NCursesPanel* output = new NCursesPanel(myStd->height()-4, myStd->width()-6, 2, 2);
    string success = "The projections files have been created successfully! Continue with <enter>";
    string failure= "You have to submit two IRC files. Continue with <enter>";
    myStd->refresh();
    output->refresh();
    myStd->boldframe("Harvest Projections");
    output->box();
    for (int i = 0; i < myOptions::options::getNumberOfFiles(); i++) {
        //Usally an IRC consists of an forward and reverse calculation. So abort when more than two files are specified (Will implement option to submit only 1 IRC)
        if (myOptions::options::getNumberOfFiles() != 2) {
            output->printw(output->height()/2, (output->width()-static_cast<int>(failure.length()))/2, "%s", failure.c_str());
            output->refresh();
            output->CUR_getch();
            delete output;
            delete myStd;
            return;
        }
        def_prog_mode();
        endwin();
        createProjections::createProjections(myOptions::options::getFileAt(i), myOptions::options::sequential(), myOptions::options::execute());
        reset_prog_mode();
    }
    
    output->printw(output->height()/2, (output->width()-static_cast<int>(success.length()))/2, "%s", success.c_str());
    output->refresh();
    output->CUR_getch();
    output->clear();
    myStd->clear();
    delete output;
    delete myStd;    
}

vector<path> fileBrowser::getDirectoryContent(const string& dir){
    path p(dir);
    try
    {
        if (is_directory(p))      // is p a directory?
        {

            typedef vector<path> vec;             // store paths,
            vec v;                                // so we can sort them later
            copy(directory_iterator(p), directory_iterator(), back_inserter(v));

            sort(v.begin(), v.end());             // sort, since directory iteration                                               // is not ordered on some file systems
            v.emplace(v.begin(), path(dir).parent_path());
            return v;
        }
    }

    catch (const filesystem_error& ex)
    {
        cout << ex.what() << '\n';
    }
}

void fileBrowser::selectItem(NCursesPanel* panel,int i){
    //move at the beginning if line
    panel->move(i,1);
    panel->clrtoeol();
    init_pair(6,COLOR_YELLOW,COLOR_BLUE);
    panel->attron(COLOR_PAIR(6));
    path curChoice=displayedFiles[i-1];
    string itemText;
    if(isSelected(curChoice))
        //itemText=selected + " " + choice + " " + curChoice.generic_string<string>();
        itemText=selected + " " + choice + " " + (--curChoice.end())->generic_string<string>();
    else if(curChoice == filelist[0])
        itemText=choice + " " + "..";
    else
        //itemText=choice + " " + curChoice.generic_string<string>();
        itemText=choice + " " + (--curChoice.end())->generic_string<string>();
    panel->printw(i,2,"%s", itemText.c_str());
    panel->attroff(COLOR_PAIR(6));
    panel->box();
    string cur_dir_s=cur_dir.string<string>();
    panel->printw(0, (panel->width()/2)-(cur_dir_s.length()/2), "%s", cur_dir_s.c_str());
    panel->refresh();
    
}

void fileBrowser::deselectItem(NCursesPanel* panel, int i){
    path curChoice=displayedFiles[i-1];
    panel->move(i,1);
    panel->clrtoeol();
    if(is_directory(curChoice)){
        panel->attron(COLOR_PAIR(3));
        if(curChoice == filelist[0])
            panel->printw(i,2,"%s","..");
        else
            panel->printw(i,2,"%s",(--curChoice.end())->generic_string<string>().c_str());
        panel->attroff(COLOR_PAIR(3));
    } else{
        string itemText;
        
        if(isSelected(curChoice))
            //itemText=selected + curChoice.string<string>();
            itemText=selected + (--curChoice.end())->generic_string<string>();
        else
            //itemText=curChoice.string<string>();
            itemText=(--curChoice.end())->generic_string<string>();
            
        panel->attron(COLOR_PAIR(4));
        panel->printw(i,2,"%s",itemText.c_str());
        panel->attroff(COLOR_PAIR(4));
        string cur_dir_s=cur_dir.string<string>();
        panel->box();
        panel->printw(0, (panel->width()/2)-(cur_dir_s.length()/2), "%s", cur_dir_s.c_str());
        
        
    }
}

bool fileBrowser::isSelected(const path &p) const{
    for(auto &f : selectedFiles){
        if(f == p)
            return true;
    }
    return false;
}

void fileBrowser::startUserLoop(NCursesPanel* panel){
    bool stop=false;
    do {
    int r=panel->CUR_getch();
    
        switch(r){
            case KEY_UP:{
                if(currentChoice == 1 && !scroll)
                    break;
                else if(currentChoice == 1 && scroll){
                    if(topLimit==0)
                        break;
                    panel->clear();
                    topLimit--;
                    updateDisplayedFiles();
                    updateDisplay(panel);
                    selectItem(panel, currentChoice);
                    break;
                }
                deselectItem(panel, currentChoice);
                selectItem(panel, --currentChoice);
                
                panel->refresh();
            }
                break;
            case KEY_DOWN:{
                if((currentChoice-1 >= displayedFiles.size()-1) && !scroll)
                    break;
                else if((currentChoice-1 >= displayedFiles.size()-1) && scroll){
                    if(displayedFiles[currentChoice-1] == filelist.back())
                        break;
                    topLimit++;
                    panel->clear();
                    updateDisplayedFiles();
                    updateDisplay(panel);
                    break;
                }
                deselectItem(panel, currentChoice);
                selectItem(panel, ++currentChoice);
                panel->refresh();
                }
                break;
            case 27: //ESC
                stop=true;
                break;
            case 10:{ // enter for enter directory
                if(is_directory(displayedFiles[currentChoice-1])){
                    filelist.clear();
                    filelist.push_back(path(".."));
                    filelist=getDirectoryContent(
                    displayedFiles[currentChoice-1].string<string>());
                    if(filelist.size() > panel->height())
                        scroll=true;
                    panel->clear();
                    cur_dir=displayedFiles[currentChoice-1];
                    currentChoice=1, topLimit=0;
                    updateDisplayedFiles();
                    updateDisplay(panel);
                }
            }
                break;
            case 32: //space for select
                if(is_regular_file(displayedFiles[currentChoice-1])){
                    selectedFiles.push_back(displayedFiles[currentChoice-1]);
                    selectItem(panel,currentChoice);
                }
                break;
            default:
                break;
        }
    }
    while(!stop);
}

void fileBrowser::updateDisplay(NCursesPanel* panel){
    int textLine=1;
    path parent=filelist[0];
    for(auto t : displayedFiles){
        
#ifdef HAVE_NCURSES_COLOR
        if(is_directory(t)){
            panel->attron(COLOR_PAIR(3));
                t+="/";
        }
        else {
            panel->attron(COLOR_PAIR(4));
        }
#endif
        path rel(t);
        if(is_regular_file(rel))
            rel=t.filename();
        else
            rel=*(--t.parent_path().end());
        if(parent == t)
            panel->printw(textLine, 2, "%s", "..");
        else
            panel->printw(textLine, 2, "%s",rel.generic_string<string>().c_str());
#ifdef HAVE_NCURSES_COLOR
        if(is_directory(t))
            panel->attroff(COLOR_PAIR(3));
        else
            panel->attroff(COLOR_PAIR(4));
#endif
        textLine++;
    }

    panel->box();
    cur_dir=system_complete(cur_dir);
    string cur_dir_s=cur_dir.string<string>();
    panel->printw(0, (panel->width()/2)-(cur_dir_s.length()/2), "%s", cur_dir_s.c_str());
    selectItem(panel,currentChoice);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    

}

void fileBrowser::updateDisplayedFiles(){
    if(!scroll){
        displayedFiles=filelist;
        return;
    }
    displayedFiles.clear();
    auto end=height+topLimit > filelist.size() ? filelist.end() : filelist.begin() + (height+topLimit);
    displayedFiles.assign(filelist.begin()+topLimit,end);
    return;
}

void fileBrowser::run(){
    NCursesPanel *std=new NCursesPanel;
    NCursesPanel *fileP=new NCursesPanel(std->height()-4, std->width()-6, 2, 2);
    string cwd;
    help::getCWD(cwd);
    filelist.clear();
    filelist=getDirectoryContent(cwd);
    cur_dir=path(cwd);
    dirname=path(cwd);
    this->currentChoice=1;
    height=fileP->height();
    std->boldframe("Select IRC files");
    std->refresh();
    fileP->box();
    init_pair(3,COLOR_BLUE,COLOR_BLACK);
    init_pair(4,COLOR_WHITE, COLOR_BLACK);
    // Are there more files than screen lines?
    if(fileP->height() < filelist.size()){
        topLimit=0;
        height=fileP->height();
        updateDisplayedFiles();
        scroll=true;
    } else{
        topLimit=0;
        height=filelist.size();
        updateDisplayedFiles();
        scroll=false;
    }
    
    updateDisplay(fileP);
    selectItem(fileP,currentChoice);
    startUserLoop(fileP);
    def_prog_mode();
    endwin();
    fileP->clear();
    std->clear();
    endwin();
    reset_prog_mode();
    delete fileP;
    delete std;
    return;
    
}
