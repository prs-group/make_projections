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

# Download the Google Test API from github
include(ExternalProject)


SET( googleTestDir ${CMAKE_CURRENT_BINARY_DIR}/googletest)
SET( googleTestInclude ${googleTestDir}/include)
SET( googleMockLibs ${googleTestDir}/lib/libgmock.a)
SET( googleTestLibs ${googleTestDir}/lib/libgtest.a)

ExternalProject_Add(googleTest
    PREFIX ${googleTestDir}
    GIT_REPOSITORY https://github.com/google/googletest.git
    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${googleTestDir}"
    )

include_directories(${googleTestInclude} ${CMAKE_SOURCE_DIR})

ADD_LIBRARY(gaussianOut STATIC EXCLUDE_FROM_ALL 
    ${CMAKE_SOURCE_DIR}/read/GaussianOut.cpp
    ${CMAKE_SOURCE_DIR}/read/FreqProjectedJob.cpp
    ${CMAKE_SOURCE_DIR}/read/IRC_Job.cpp
    ${CMAKE_SOURCE_DIR}/options/options.cpp
    ${CMAKE_SOURCE_DIR}/read/regex.cpp
    )

ADD_SUBDIRECTORY("dft")
ADD_SUBDIRECTORY("options")
