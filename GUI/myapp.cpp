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


#include "myapp.h"
#include <stdio.h>
#include <cstring>

void TestApplication::title()
{
    const char * const titleText = "Make Projection";
    const int len = static_cast<int>(::strlen(titleText));
    
    titleWindow->bkgd(screen_titles());
    titleWindow->addstr(0, (titleWindow->cols() - len)/2, titleText);
    titleWindow->noutrefresh();
}
	
void TestApplication::init_labels(Soft_Label_Key_Set& S) const
{
    for(int i=1; i <= S.labels(); i++) {
        char buf[8];
        assert(i < 100);
        ::_nc_SPRINTF(buf, _nc_SLIMIT(sizeof(buf)) "Key%02d", i);
        S[i] = buf;                                      // Text
        S[i] = Soft_Label_Key_Set::Soft_Label_Key::Left; // Justification
    }
}

int TestApplication::run()
{
    MyMenu M;
    M();
    return 0;
}
