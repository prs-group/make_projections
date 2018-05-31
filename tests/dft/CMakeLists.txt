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

# Add custom test command
ADD_EXECUTABLE(executeTest IMPORTED)
SET_TARGET_PROPERTIES(executeTest PROPERTIES IMPORTED_LOCATION '${CMAKE_SOURCE_DIR}/tests/executeTest.sh')
# --------------------------------------------------------------------
# MeCOH B3LYP gaussian09-A #p flag 
# --------------------------------------------------------------------
include(TestingMacros)
add_gtest(NAME dft_test1 EXECUTABLE test1 SOURCES "test1.cpp")
TARGET_LINK_LIBRARIES(test1 gaussianOut yaml-cpp)
TARGET_COMPILE_DEFINITIONS(test1 PUBLIC "-DIRC_FORWARD_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/test_1_f.out\"")
TARGET_COMPILE_DEFINITIONS(test1 PUBLIC "-DIRC_REVERSE_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/test_1_r.out\"")
    
#add_test(NAME dft_test1 COMMAND test1 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

