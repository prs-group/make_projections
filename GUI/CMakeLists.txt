# This file is part of make_projections.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# laster any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# make_projections 1.0.0 
# Copyright (c) Henrik Quanz

set(SOURCE
    ${SOURCE}
    ${CMAKE_CURRENT_SOURCE_DIR}/myapp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mymenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mymenuItems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AbstractTableView.cpp
    PARENT_SCOPE
    )

set(HEADERS
    ${HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/myapp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mymenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mymenuItems.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AbstractTableView.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AbstractTableView_p.h
    PARENT_SCOPE
    )
