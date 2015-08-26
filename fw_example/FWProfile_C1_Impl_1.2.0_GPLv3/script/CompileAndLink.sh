#!/bin/bash
# This script compiles and links the following programs for the C1 Implementation:
# - The Test Suite
# - The Demo Application
# - The Coding Example Programs
#
# The script takes the following parameters:
# 1. The path of the FW Profile directory 
# 2. The path of the directory where the Test Suite source files are located
# 3. The path of the directory where the Demo Application source files are located
# 4. The path of the directory where the State Machine Coding Example source files are located
# 5. The path of the directory where the Procedure Coding Example source files are located
# 6. The path of the directory where the RT Container Coding Example source files are located
# 7. The path of the directory where the object and executable files are built
#
# This script performs the following actions:
# 1. Compile all the C1 Implementation files
# 2. Compile the Test Suite Files for the C1 Implementation
# 3. Build the executable to run the Test Suite for the C1 Implementation
# 4. Compile the Demo Application Files 
# 5. Build the executable to run the Demo Application for the C1 Implementation
# 6. Build the executable to run the all coding examples for the C1 Implementation
#
# In all cases, compilation and linking is done with the gcov options.
#
#====================================================================================
# Assign variables (OBJ_DIR and EXE_DIR must be the same directory)
#====================================================================================
set -xv

SRC=$1
FW_SRC="$SRC/FwProfile"
FW_SRC_TS=$2
FW_SRC_SM_APP=$3
FW_SRC_SM_TUTORIALS=$4
FW_SRC_PR_TUTORIALS=$5
FW_SRC_RT_TUTORIALS=$6
OBJ_DIR=$7
EXE_DIR=$7

#echo "===================================================================================="
echo "- Compile all the C1 Implementation Files"
#echo "===================================================================================="
# OPT="-Os -Wall -c -fmessage-length=0"  #<-- use this for linker map
OPT="-O0 -g3 -Wall -c -fmessage-length=0 -fprofile-arcs -ftest-coverage"   
gcc $OPT -o $OBJ_DIR/FwPrConfig.o $FW_SRC/FwPrConfig.c
gcc $OPT -o $OBJ_DIR/FwPrCore.o $FW_SRC/FwPrCore.c
gcc $OPT -o $OBJ_DIR/FwPrDCreate.o $FW_SRC/FwPrDCreate.c
gcc $OPT -o $OBJ_DIR/FwPrSCreate.o $FW_SRC/FwPrSCreate.c
gcc $OPT -o $OBJ_DIR/FwSmAux.o $FW_SRC/FwSmAux.c
gcc $OPT -o $OBJ_DIR/FwSmConfig.o $FW_SRC/FwSmConfig.c
gcc $OPT -o $OBJ_DIR/FwSmCore.o $FW_SRC/FwSmCore.c
gcc $OPT -o $OBJ_DIR/FwSmDCreate.o $FW_SRC/FwSmDCreate.c
gcc $OPT -o $OBJ_DIR/FwSmSCreate.o $FW_SRC/FwSmSCreate.c
gcc $OPT -o $OBJ_DIR/FwRtConfig.o $FW_SRC/FwRtConfig.c
gcc $OPT -o $OBJ_DIR/FwRtCore.o $FW_SRC/FwRtCore.c

#echo "===================================================================================="
echo "- Compile all the Test Suite Files"
#echo "===================================================================================="
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwPrMakeTest.o $FW_SRC_TS/FwPrMakeTest.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwPrTestCases.o $FW_SRC_TS/FwPrTestCases.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwSmMakeTest.o $FW_SRC_TS/FwSmMakeTest.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwSmTestCases.o $FW_SRC_TS/FwSmTestCases.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwRtMakeTest.o $FW_SRC_TS/FwRtMakeTest.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwRtTestCases.o $FW_SRC_TS/FwRtTestCases.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwTestSuite.o $FW_SRC_TS/FwTestSuite.c

#echo "===================================================================================="
echo "- Build the executable to run the Test Suite"
#echo "===================================================================================="
#LNKMAP="-Wl,-Map,$EXE_DIR/sm_test.map" #<-- use this for linker map (and remove -fprofile-arcs option)
LNKMAP=""
gcc -fprofile-arcs -o $EXE_DIR/sm_test $OBJ_DIR/FwPrConfig.o $OBJ_DIR/FwPrCore.o $OBJ_DIR/FwPrDCreate.o $OBJ_DIR/FwPrSCreate.o $OBJ_DIR/FwSmAux.o $OBJ_DIR/FwSmConfig.o $OBJ_DIR/FwSmCore.o $OBJ_DIR/FwSmDCreate.o $OBJ_DIR/FwSmSCreate.o $OBJ_DIR/FwRtConfig.o $OBJ_DIR/FwRtCore.o $OBJ_DIR/FwPrMakeTest.o $OBJ_DIR/FwPrTestCases.o $OBJ_DIR/FwSmMakeTest.o $OBJ_DIR/FwSmTestCases.o $OBJ_DIR/FwRtMakeTest.o $OBJ_DIR/FwRtTestCases.o $OBJ_DIR/FwTestSuite.o -lpthread $LNKMAP

#echo "===================================================================================="
echo "- Compile all the Demo Application Files"
#echo "===================================================================================="
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwDaCurCheck.o $FW_SRC_SM_APP/FwDaCurCheck.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwDaCurRecAction.o $FW_SRC_SM_APP/FwDaCurRecAction.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwDaDeltaCheck.o $FW_SRC_SM_APP/FwDaDeltaCheck.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwDaFDCheck.o $FW_SRC_SM_APP/FwDaFDCheck.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwDaHwDev.o $FW_SRC_SM_APP/FwDaHwDev.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwDaMain.o $FW_SRC_SM_APP/FwDaMain.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwDaTempCheck.o $FW_SRC_SM_APP/FwDaTempCheck.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwDaTempRecAction.o $FW_SRC_SM_APP/FwDaTempRecAction.c

#echo "===================================================================================="
echo "- Build the executable to run the Demo Application"
#echo "===================================================================================="
gcc -fprofile-arcs -o $EXE_DIR/sm_demo $OBJ_DIR/FwPrConfig.o $OBJ_DIR/FwPrCore.o $OBJ_DIR/FwPrDCreate.o $OBJ_DIR/FwPrSCreate.o $OBJ_DIR/FwSmAux.o $OBJ_DIR/FwSmConfig.o $OBJ_DIR/FwSmCore.o $OBJ_DIR/FwSmDCreate.o $OBJ_DIR/FwSmSCreate.o $OBJ_DIR/FwDaCurCheck.o $OBJ_DIR/FwDaCurRecAction.o $OBJ_DIR/FwDaDeltaCheck.o $OBJ_DIR/FwDaFDCheck.o $OBJ_DIR/FwDaHwDev.o $OBJ_DIR/FwDaMain.o $OBJ_DIR/FwDaTempCheck.o $OBJ_DIR/FwDaTempRecAction.o

#echo "===================================================================================="
echo "- Compile all Coding Examples"
#echo "===================================================================================="
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwProfile_SmExample1.o $FW_SRC_SM_TUTORIALS/FwProfile_SmExample1.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwProfile_SmExample2.o $FW_SRC_SM_TUTORIALS/FwProfile_SmExample2.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwProfile_SmExample3.o $FW_SRC_SM_TUTORIALS/FwProfile_SmExample3.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwProfile_PrExample1.o $FW_SRC_PR_TUTORIALS/FwProfile_PrExample1.c
gcc -I$SRC -O0 -g3 -Wall -c -fmessage-length=0 -o $OBJ_DIR/FwProfile_RtExample1.o $FW_SRC_RT_TUTORIALS/FwProfile_RtExample1.c

#echo "===================================================================================="
echo "- Build the executable to run Coding Examples"
#echo "===================================================================================="
gcc -fprofile-arcs -o $EXE_DIR/sm_example1 $OBJ_DIR/FwSmAux.o $OBJ_DIR/FwSmConfig.o $OBJ_DIR/FwSmCore.o $OBJ_DIR/FwSmDCreate.o $OBJ_DIR/FwSmSCreate.o $OBJ_DIR/FwProfile_SmExample1.o
gcc -fprofile-arcs -o $EXE_DIR/sm_example2 $OBJ_DIR/FwSmAux.o $OBJ_DIR/FwSmConfig.o $OBJ_DIR/FwSmCore.o $OBJ_DIR/FwSmDCreate.o $OBJ_DIR/FwSmSCreate.o $OBJ_DIR/FwProfile_SmExample2.o
gcc -fprofile-arcs -o $EXE_DIR/sm_example3 $OBJ_DIR/FwSmAux.o $OBJ_DIR/FwSmConfig.o $OBJ_DIR/FwSmCore.o $OBJ_DIR/FwSmDCreate.o $OBJ_DIR/FwSmSCreate.o $OBJ_DIR/FwProfile_SmExample3.o
gcc -fprofile-arcs -o $EXE_DIR/pr_example1 $OBJ_DIR/FwPrConfig.o $OBJ_DIR/FwPrCore.o $OBJ_DIR/FwPrDCreate.o $OBJ_DIR/FwPrSCreate.o $OBJ_DIR/FwProfile_PrExample1.o
gcc -fprofile-arcs -o $EXE_DIR/rt_example1 $OBJ_DIR/FwRtConfig.o $OBJ_DIR/FwRtCore.o $OBJ_DIR/FwProfile_RtExample1.o -lpthread

#echo "===================================================================================="

