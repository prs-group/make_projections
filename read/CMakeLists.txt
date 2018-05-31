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
    ${CMAKE_CURRENT_SOURCE_DIR}/FBIRC.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FreqProjectedJob.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GaussianOut.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IRC_Job.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IRCProjectedOut.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/regex.cpp
    PARENT_SCOPE
    )

set(HEADERS
    ${HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/createProjections.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FBIRC.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FreqProjectedJob.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GaussianOut.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GaussReading.h
    ${CMAKE_CURRENT_SOURCE_DIR}/harvest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/IRC_Job.h
    ${CMAKE_CURRENT_SOURCE_DIR}/IRCProjectedOut.h
    ${CMAKE_CURRENT_SOURCE_DIR}/createHighLevelSP.h
    ${CMAKE_CURRENT_SOURCE_DIR}/regex.h
    PARENT_SCOPE
    )

