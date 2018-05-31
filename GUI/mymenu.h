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


#ifndef Tunneling_Analysis_mymenu_h
#define Tunneling_Analysis_mymenu_h

#include "config.h"

#include <cursesm.h>
#include <cursesw.h>
#include <cursesapp.h>
#include <cursesf.h>


#include <string>
#include "mymenuItems.h"
#include "options/options.h"


class MyMenu : public NCursesMenu
{
private:
    NCursesPanel* P;
    NCursesMenuItem** I;
    
#define n_items 6
    
public:
    MyMenu ();
    
    MyMenu& operator=(const MyMenu& rhs);
    
    MyMenu(const MyMenu& rhs)
    : NCursesMenu(rhs), P(0), I(0)
    {
    }
    
    ~MyMenu();
    
    virtual void On_Menu_Init();
    
    virtual void On_Menu_Termination();
    
    virtual void On_Item_Init(NCursesMenuItem& item);
    
    virtual void On_Item_Termination(NCursesMenuItem& item);
};

#endif
