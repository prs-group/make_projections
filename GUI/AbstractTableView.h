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

#include "../help/HelpFunctions.h"
#include <string>
#include <vector>

namespace NCursesGUI{
    class Variant{
    private:
        class VariantPrivate;
        DECLARE_PRIVATE(Variant)
        VariantPrivate *d_ptr;
    public:
        Variant();
        Variant(int v);
        Variant(float v);
        Variant(double v);
        Variant(long double v);
        Variant(char c);
        Variant(std::string* c);
        Variant(std::vector<std::string>* v);
        
        void setValue(int v);
        void setValue(float v);
        void setValue(double v);
        void setValue(long double v);
        void setValue(char c);
        void setValue(std::string* c);
        void setValue(std::vector<std::string>* v);
        
        int toInt(bool &ok);
        float toFloat(bool &ok);
        double toDouble(bool &ok);
        long double toLDouble(bool &ok);
        char toChar(bool &ok);
        std::vector<std::string>* toStdVector(bool &ok);
        std::string* toStdString(bool &ok);
    }; //Variant

    class AbstractTableView{
    private:
        class AbstractTableViewPrivate;
        DECLARE_PRIVATE(AbstractTableView)
        AbstractTableViewPrivate *d_ptr;

    public:
        AbstractTableView();
        virtual int numberOfRows() = 0;
        virtual int numberOfColums() = 0;
    }; //Abstract Table View
}; // Namespace
