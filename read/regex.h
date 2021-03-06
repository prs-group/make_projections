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

#include <boost/regex.hpp>

#ifndef READ_CMAKEFILES_REGEX_H_
#define READ_CMAKEFILES_REGEX_H_

namespace regex {
	namespace gaussian {
		extern boost::regex atomicSymbols;
		extern boost::regex scfEnergy;
		extern boost::regex mpEnergy;
		extern boost::regex geometry;
		extern boost::regex reactionCoord;
		extern boost::regex pointNumber;
	}
}



#endif /* READ_CMAKEFILES_REGEX_H_ */
