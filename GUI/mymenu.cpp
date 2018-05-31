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


#include "mymenu.h"
#include "../read/harvest.h"
#include "../config/Config.h"

MyMenu::MyMenu ()
: NCursesMenu (n_items+2, 27, (lines()-10)/2, (cols()-25)/2),
P(0), I(0)
{

    I = new NCursesMenuItem*[1+n_items];
    I[0] = new selectFiles("Select IRCs");
    I[1] = new IRCAction("write IRC");
    I[2] = new showIRCFile("show IRC file");
    I[3] = new createTheProjection("create Projection files");
    I[4] = new harvestProjections("harvest projection files");
    I[5] = new QuitItem();
    I[6] = new NCursesMenuItem(); // Terminating empty item
    
    InitMenu(I, TRUE, TRUE);
    
    P = new NCursesPanel(1, n_items, LINES-1, 1);
    boldframe("Make", "Projection");
    P->show();
}

MyMenu::~MyMenu()
{
    P->hide();
    delete P;
}

MyMenu& MyMenu::operator=(const MyMenu& rhs)
{
    if (this != &rhs) {
        *this = rhs;
    }
    return *this;
}

 void MyMenu::On_Menu_Init()
{
    NCursesWindow W(::stdscr);
    P->move(0, 0);
    P->clrtoeol();
    for(int i=1; i<=count(); i++)
        P->addch('0' + i);

    P->refresh();
}

void MyMenu::On_Menu_Termination()
{
    P->move(0, 0);
    P->clrtoeol();
    P->refresh();
}

void MyMenu::On_Item_Init(NCursesMenuItem& item)
{
    P->move(0, item.index());
    P->attron(A_REVERSE);
    P->printw("%1d", 1+item.index());
    P->attroff(A_REVERSE);
    const std::string name = item.name();
    if( name == "harvest projection files" ){
        Config conf;
        if(!harvest::checkHarvestFiles(conf.getGaussFileEnding())){
            item.options_off(O_SELECTABLE);
        }
        else
            item.options_on(O_SELECTABLE);
    }
    P->refresh();
}


void MyMenu::On_Item_Termination(NCursesMenuItem& item)
{
    P->move(0, item.index());
    P->attroff(A_REVERSE);
    P->printw("%1d", 1+item.index());
    const std::string name = item.name();
    if( name == "harvest projection files" ){
        Config conf;
        if(!harvest::checkHarvestFiles(conf.getGaussFileEnding())){
            item.options_off(O_SELECTABLE);
        }
        else
            item.options_on(O_SELECTABLE);
    }
    P->refresh();
}
