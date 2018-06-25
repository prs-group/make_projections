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

#include "regex.h"


using namespace regex;

boost::regex gaussian::atomicSymbols(
		R"(^\s+(?!Charge)(?!Symbolic)(\w+)(\(iso=\d+\))?)",
		boost::regex::perl | boost::regex::icase);

boost::regex gaussian::scfEnergy(R"(^\s+SCF Done:\s+E\(\w+\)\s+=\s+([+-]?\d+\.?\d+))");

boost::regex gaussian::mpEnergy(R"(^\s*E2\(?\w*\)?\s+=\s+[+-]?\d+\.\d+D[+-]\d+\s+E\(?\w+\)?\s+=\s+([+-]?\d+\.\d+D[+-]\d+))");

boost::regex gaussian::geometry("^\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+"
                                "([+-]?\\d+\\.?\\d+)\\s+"
                                "([+-]?\\d+\\.?\\d+)\\s+"
                              "([+-]?\\d+\\.?\\d+)");

boost::regex gaussian::reactionCoord("^\\s+NET REACTION COORDINATE UP TO THIS POINT"
                            "\\s+=\\s+([+-]?\\d+\\.?\\d+)");

boost::regex gaussian::pointNumber("^\\s+Point Number:\\s+(\\d+)\\s+Path Number:\\s+(\\d+)");

