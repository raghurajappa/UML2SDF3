#!/bin/bash
# This script builds the Test Suite, the Demo Application and the Coding Example Programs
# for the C1 Implementation of the FW Profile.
#
# The script should be run from the /script directory of the C1 Implementation delivery.
#
# The script performs the following actions:
# 1. It compiles all the C1 Implementation source files
# 2. It creates the executables for the Test Suite (sm_test), for the Demo Application
#    (sm_demo), and for the Coding Example programs (sm_example1, sm_example2, sm_example3
#    and pr_example1).
#
# By default, both the executables and their object files are created in the /script
# sub-directory of the C1 Implementation delivery. User can change this target directory
# by overriding the EXE shell variable at the beginning of the script.nmn

#echo "===================================================================================="
#echo "- Define directory where object and executable files are created"
#echo "===================================================================================="
EXE="."

#echo "===================================================================================="
#echo "- Define directories where source files are located"
#echo "===================================================================================="
SRC="../src"
FW_SRC_TS="../src/FwProfile_TestSuite"
FW_SRC_SM_APP="../src/FwProfile_SM_App"
FW_SRC_SM_TUTORIALS="../src/FwProfile_SM_Tutorials"
FW_SRC_PR_TUTORIALS="../src/FwProfile_PR_Tutorials"
FW_SRC_RT_TUTORIALS="../src/FwProfile_RT_Tutorials"

#echo "===================================================================================="
echo "- Compile and link all sources"
#echo "===================================================================================="
./CompileAndLink.sh $SRC $FW_SRC_TS $FW_SRC_SM_APP $FW_SRC_SM_TUTORIALS $FW_SRC_PR_TUTORIALS $FW_SRC_RT_TUTORIALS $EXE


