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

#include "AbstractTableView_p.h"
#include <string>
#include <vector>

using namespace NCursesGUI;


Variant::Variant(): d_ptr(new VariantPrivate){
}
Variant::Variant(int v): d_ptr(new VariantPrivate(v)){
}
Variant::Variant(float v): d_ptr(new VariantPrivate(v)){
}
Variant::Variant(double v): d_ptr(new VariantPrivate(v)){
}
Variant::Variant(long double v): d_ptr(new VariantPrivate(v)){
}
Variant::Variant(char v): d_ptr(new VariantPrivate(v)){
}
Variant::Variant(std::string* v): d_ptr(new VariantPrivate(v)){
}
Variant::Variant(std::vector<std::string>* v): d_ptr(new VariantPrivate(v)){
}
        
void Variant::setValue(int v){
    D(Variant)
    d->setValue(v);
}
void Variant::setValue(float v){
    D(Variant)
    d->setValue(v);
}
void Variant::setValue(double v){
    D(Variant)
    d->setValue(v);
}
void Variant::setValue(long double v){
    D(Variant)
    d->setValue(v);
}
void Variant::setValue(char v){
    D(Variant)
    d->setValue(v);
}
void Variant::setValue(std::string* v){
    D(Variant)
    d->setValue(v);
}
void Variant::setValue(std::vector<std::string>* v){
    D(Variant)
    d->setValue(v);
}
        
int Variant::toInt(bool &ok){
    D(Variant)
    return d->toInt(ok);
}
float Variant::toFloat(bool &ok){
    D(Variant)
    return d->toFloat(ok);
}
double Variant::toDouble(bool &ok){
    D(Variant)
    return d->toDouble(ok);
}
long double Variant::toLDouble(bool &ok){
    D(Variant)
    return d->toLDouble(ok);
}
char Variant::toChar(bool &ok){
    D(Variant)
    return d->toChar(ok);
}
std::vector<std::string>* Variant::toStdVector(bool &ok){
    D(Variant)
    return d->toStdVector(ok);
}
std::string* Variant::toStdString(bool &ok){
    D(Variant)
    return d->toStdString(ok);
}

// End of Variant Implementation

AbstractTableView::AbstractTableView():d_ptr(new AbstractTableViewPrivate){
} // End of Abstract Table View Implementation

