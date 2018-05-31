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


#ifndef Tunneling_Analysis_myapp_h
#define Tunneling_Analysis_myapp_h

#include "config.h"

#include "mymenu.h"
#include <cursesapp.h>
#include <cursesf.h>


#ifdef __cplusplus
#define NCURSES_VOID /* nothing */
#else
#define NCURSES_VOID (void)
#endif

#ifdef USE_STRING_HACKS && HAVE_SNPRINTF
#define _nc_SPRINTF             NCURSES_VOID snprintf
#define _nc_SLIMIT(n)           (n),
#else
#define _nc_SPRINTF             NCURSES_VOID sprintf
#define _nc_SLIMIT(n)		/* nothing */
#endif

#ifdef __MINGW32__
#undef KEY_EVENT
#endif

#ifndef __MINGW32__
extern "C" unsigned int sleep(unsigned int);
#endif

#undef index // needed for NeXT

#ifdef __gnu_linux__
#define NULL
#endif

class TestApplication : public NCursesApplication
{
private:

protected:
    int titlesize() const { return 1; }
    void title();
    Soft_Label_Key_Set::Label_Layout useSLKs() const {
        return Soft_Label_Key_Set::PC_Style_With_Index;
    }
    void init_labels(Soft_Label_Key_Set& S) const;
    
public:
#ifdef HAVE_NCURSES_COLOR
    TestApplication() : NCursesApplication(TRUE){
    }
#else
    TestApplication() : NCursesApplication(FALSE){
    }
#endif
    
    int run();
};




#endif
