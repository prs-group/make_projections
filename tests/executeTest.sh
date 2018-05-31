#!/bin/bash

# Argument parsing
# 1st: Executable 
# 2nd: IRC forward
# 3rd: IRC reverse
# 4th: LogFile
# 5th: Reference IRC.txt file
# 6th: Reference IRC_projected.txt file

exe=$1
shift
irc_f=$1
shift
irc_r=$1
shift
logFile=$1
shift
ref_IRC_file=$1
shift
ref_IRC_projFile=$1
shift

# Basename of test File
base=$(basename -s .com $irc_f)

fileExists () {
	local file=$1
	if [ ! -f $file ]; then
		echo "File $file does not exists!" 
		exit -1
	fi
}

checkExitCode () {
    local exitCode=$1
    	if [ $exitCode -ne 0 ]; then
		exit $exitCode
	fi
}



# Checking arguments
fileExists $irc_f
fileExists $irc_r
fileExists $ref_IRC_file
# TO DO: Not implemented yet!
#fileExists $ref_IRC_projFile




# Test 1 Extracting IRC from Gaussian Files
# DO TO: Write test that compares referecne file
# with generated file
echo "Writing IRC..." > $logFile
$exe -w $base $irc_f $irc_r &> $logFile
checkExitCode $#

# Test 2 Writing IRC to Gaussian file
# Don't use default parameters for parsing test
# DO TO: Write test that compares referecne file
# with generated fiel
echo "Writing projected files..." > $logFile
$exe -m 1024 -p 2 $irc_f $irc_r &> $logFile
checkExitCode $#

# Test 3 Reading projecting files
# TO DO: Write Test 3
