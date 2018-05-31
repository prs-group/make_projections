macro(add_my_test _name)
    # The make_projection executable
    set(PROJ_EXE "make_projection_test")
    set(TEST_CMD "executeTest")
    # A full log
    set(LOGFILE ${PROJECT_BINARY_DIR}/testresults.log)
    # Generic Setup
    set(TEST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    set(IRC_FORWARD  ${_name}_f.out)
    set(IRC_REVERSE  ${_name}_r.out)
    set(IRC_FILE_REF ${TEST_SRC_DIR}/IRC_${_name}.ref)
    set(IRC_PROJ_REF ${TEST_SRC_DIR}/IRC_projected_${_name}.ref)
    # Some tests are in subdirectories of subdirectories (cfour, mrcc, dftd3) 
    get_filename_component(dir ${TEST_SRC_DIR} NAME)
    get_filename_component(dir ${dir} NAME)
    set(TEST_RUN_DIR ${PROJECT_BINARY_DIR}/tests/${dir}/)
    file(MAKE_DIRECTORY ${TEST_RUN_DIR})
    
    # Copy files to destination (because program does not have
    # function to read/write files into different directories
    # (maybe debug build needed)
    file(COPY ${TEST_SRC_DIR}/${IRC_FORWARD} DESTINATION ${TEST_RUN_DIR})
    file(COPY ${TEST_SRC_DIR}/${IRC_REVERSE} DESTINATION ${TEST_RUN_DIR}) 
    
    add_test(NAME "${_name}" COMMAND ${TEST_CMD} ${PROJ_EXE} 
        ${IRC_FORWARD} ${IRC_REVERSE} ${LOGFILE} ${IRC_FILE_REF} ${IRC_PROJ_REF}
        WORKING_DIRECTORY ${TEST_RUN_DIR})
   
   SET_TESTS_PROPERTIES("${name_}" PROPERTIES TIMEOUT 2) 
    
ENDMACRO()


MACRO(add_gtest)
    set(options "")
    set(oneValueArgs NAME EXECUTABLE)
    set(multiValueArgs SOURCES ARGUMENTS)
    cmake_parse_arguments(add_gtest "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    add_executable(${add_gtest_EXECUTABLE} ${add_gtest_SOURCES})
    ADD_DEPENDENCIES(${add_gtest_EXECUTABLE} googleTest)
    TARGET_LINK_LIBRARIES(${add_gtest_EXECUTABLE} ${googleTestLibs} ${CORELIBS}) 
    add_test(NAME ${add_gtest_NAME}
        COMMAND ${add_gtest_EXECUTABLE} ${add_gtest_ARGUMENTS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    
ENDMACRO()
   
   