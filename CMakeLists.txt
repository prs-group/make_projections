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

cmake_minimum_required(VERSION 3.3)
project(make_projection_project)
# Enable Testing when Flag is set
if(TESTING)
enable_testing()
endif(TESTING)
# Include Modules
list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake/testing
    )
# Options
option(USE_STRING_HACKS "Use string hacks" ON)
option(WITH_NCURSES "Compile with ncurses library for GUI support" OFF)

INCLUDE(CheckCXXSymbolExists)
INCLUDE(ExternalProject)


# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS                "-Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER "4.7" OR GCC_VERSION VERSION_EQUAL "4.7"))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else ()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()


  
set(HEADERS
    ${HEADERS}
    ${PROJECT_BINARY_DIR}/config.h
    )

# --------------------------------------------------------------------
# Add the directories of all CMakeFiles without the GUI folder
# because the GUI is only compiled when specify -WITH-NCURSES=OFF
# --------------------------------------------------------------------

#add_subdirectory("${PROJECT_SOURCE_DIR}/yaml-lib")
add_subdirectory("${PROJECT_SOURCE_DIR}/config")
add_subdirectory("${PROJECT_SOURCE_DIR}/Data_Analysis")
add_subdirectory("${PROJECT_SOURCE_DIR}/help")
add_subdirectory("${PROJECT_SOURCE_DIR}/options")
add_subdirectory("${PROJECT_SOURCE_DIR}/read")

# --------------------------------------------------------------------
# Build the yaml-lib as external project
# --------------------------------------------------------------------
set(YAML_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/external")
if(TESTING)
	set(YAML_CMAKE_ARGS "-DYAML_CPP_BUILD_TESTS=ON;${YAML_CMAKE_ARGS}")
else()
	set(YAML_CMAKE_ARGS "-DYAML_CPP_BUILD_TESTS=OFF;${YAML_CMAKE_ARGS}")
endif(TESTING)

ExternalProject_Add(yaml-cpp-project
	GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
	GIT_TAG yaml-cpp-0.6.2 
	GIT_PROGRESS true
	STEP_TARGETS build
	CMAKE_ARGS ${YAML_CMAKE_ARGS})

ExternalProject_Get_Property(yaml-cpp-project install_dir)

     

set(CURSES_NEED_NCURSES TRUE)
find_package(Boost 1.49.0 REQUIRED COMPONENTS regex filesystem system program_options)
find_package(Curses)


include_directories(${Boost_INCLUDE_DIR} 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/external/include)


set(SOURCE
    ${SOURCE}
    ${PROJECT_SOURCE_DIR}/main.cpp
    )

# --------------------------------------------------------------------
# Ncurses is configured here
# --------------------------------------------------------------------
if(WITH_NCURSES)
	set(HAVE_NCURSES_H OFF)

	if(CURSES_FOUND)
    		find_library(CURSESPP ncurses++)
    		find_library(CURSESW ncursesw)
    		find_library(CURSESM menu)
    		find_library(CURSESP panel)
    		set(CURSES_LIBRARIES
        	${CURSES_LIBRARIES}
        	${CURSESPP}
        	${CURSESW}
        	${CURSESM}
        	${CURSESP}
        	)
		include_directories(${CURSES_INCLUDE_PATH})
	find_path(CURSES_CPP_H cursesapp.h cursesf.h cursesw.h cursesm.h)
    	if(DEFINED ${CURSES_CPP_H}})
    		message(WARNING "NCurses c++ bindings not found! NCurses will be disabled!")
		set(CURSES_LIBRARY "")
    	else()
    		set(HAVE_NCURSES_H ON)
    		add_subdirectory("${PROJECT_SOURCE_DIR}/GUI")
    		endif()

	endif(CURSES_FOUND)

# Check for the include dir either ncurses/.h or the root folder
	set(HAVE_NCURSESCPP_H OFF)
	set(HAVE_NCURSES_CURSESCPP_H OFF)

	if(EXISTS ${CURSES_HAVE_NCURSES_H})
    		set(HAVE_NCURSESCPP_H ON)
	elseif(EXISTS ${CURSES_HAVE_NCURSES_NCURSES_H})
    		set(HAVE_NCURSES_CURSESCPP_H ON)
	endif()


	set(LINK_LIBRARIES "${CURSES_LIBRARIES}")
	set(INCLUDE_DIRECTORIES ${CURSES_INCLUDE_DIR} ${CURSES_CPP_H})
	set(LINK_DIRECTORIES ${CMAKE_PREFIX_PATH})
	if(HAVE_NCURSES_H)
		set(COMPILE_DEFINITIONS "-DHAVE_NCURSES_H")
	else()
		set(COMPILE_DEFINITIONS "-DHAVE_NCURSES_CURSES_H")
	endif()

	message(STATUS "Check if the curses library supports colors: ")
	try_run(NCURSES_COLOR COMPILE_RESULT "${CMAKE_BINARY_DIR}/CMakeFiles/CMakeTmp"
		${CMAKE_SOURCE_DIR}/CMakeTests/curses_colors.c
	COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS}
	LINK_LIBRARIES ${LINK_LIBRARIES})

	if(NOT ${NCURSES_COLOR})
    		set(HAVE_NCURSES_COLOR ON)
	else()
    		set(HAVE_NCURSES_COLOR OFF)
	endif()

	message(STATUS "  ${HAVE_NCURSES_COLOR}")
else()
	set(CURSES_LIBRARIES "")
endif(WITH_NCURSES)


CHECK_CXX_SYMBOL_EXISTS(snprintf stdio.h HAVE_SNPRINTF)

configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
  

set(CORELIBS ${CURSES_LIBRARIES} ${Boost_LIBRARIES})
set(yaml-cpp-lib ${CMAKE_BINARY_DIR}/external/lib/libyaml-cpp${CMAKE_STATIC_LIBRARY_SUFFIX})

add_executable(make_projection ${SOURCE} ${HEADERS})
add_executable(make_projection_test ${SOURCE} ${HEADERS})
SET_TARGET_PROPERTIES(make_projection_test PROPERTIES COMPILE_FLAGS "-DTEST")
add_dependencies(make_projection yaml-cpp-project)
add_dependencies(make_projection_test yaml-cpp-project)
target_link_libraries(make_projection ${yaml-cpp-lib} ${CORELIBS})
target_link_libraries(make_projection_test ${yaml-cpp-lib} ${CORELIBS})
INSTALL(TARGETS make_projection
  RUNTIME DESTINATION bin
)

# Testing must be appended in order get all the variables
if(TESTING)
ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/tests")
ENDIF(TESTING)

