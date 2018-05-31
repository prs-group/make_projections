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

#include "AbstractTableView.h"

using namespace NCursesGUI;

class Variant::VariantPrivate{
    private:
        DECLARE_PUBLIC(Variant)
        Variant* q_ptr;
//        void *value;
        std::string type;
    public:
        VariantPrivate();
        VariantPrivate(int v);
        VariantPrivate(float v);
        VariantPrivate(double v);
        VariantPrivate(long double v);
        VariantPrivate(char c);
        VariantPrivate(std::string* c);
        VariantPrivate(std::vector<std::string>* v);
        
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

}; // End of VariantPrivate Declaration

// Begin of VariantPrivate Implementation

Variant::VariantPrivate::VariantPrivate(){
}
Variant::VariantPrivate::VariantPrivate(int v){
}
Variant::VariantPrivate::VariantPrivate(float v){
}
Variant::VariantPrivate::VariantPrivate(double v){
}
Variant::VariantPrivate::VariantPrivate(long double v){
}
Variant::VariantPrivate::VariantPrivate(char c){
}
Variant::VariantPrivate::VariantPrivate(std::string* c){
}
Variant::VariantPrivate::VariantPrivate(std::vector<std::string>* v){
}
        
void Variant::VariantPrivate::setValue(int v){
}
void Variant::VariantPrivate::setValue(float v){
}
void Variant::VariantPrivate::setValue(double v){
}
void Variant::VariantPrivate::setValue(long double v){
}
void Variant::VariantPrivate::setValue(char c){
}
void Variant::VariantPrivate::setValue(std::string* c){
}
void Variant::VariantPrivate::setValue(std::vector<std::string>* v){
}
        
int Variant::VariantPrivate::toInt(bool &ok){
    return 0;
}
float Variant::VariantPrivate::toFloat(bool &ok){
    return 0.0f;
}
double Variant::VariantPrivate::toDouble(bool &ok){
    return 0.0;
}
long double Variant::VariantPrivate::toLDouble(bool &ok){
    return 0.0;
}
char Variant::VariantPrivate::toChar(bool &ok){
    return '0';
}
std::vector<std::string>* Variant::VariantPrivate::toStdVector(bool &ok){
    std::vector<std::string>* v=new std::vector<std::string>();
    return v;
}
std::string* Variant::VariantPrivate::toStdString(bool &ok){
    std::string *s=new std::string();
    return s;
}


class AbstractTableView::AbstractTableViewPrivate{
    private:
    AbstractTableView *q_ptr;
    DECLARE_PUBLIC(AbstractTableViewPrivate)
    public:
    AbstractTableViewPrivate();
};// AbstractTableViewPrivate

AbstractTableView::AbstractTableViewPrivate::AbstractTableViewPrivate(){
} 

// End of Abstract TableView Implementations
