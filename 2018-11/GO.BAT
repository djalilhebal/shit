REM This script is used by retardedAO.js

echo off
cls

echo Cleaning...
if exist PROGRAM.EXE del PROGRAM.EXE
if exist PROGRAM.OBJ del PROGRAM.OBJ

echo Assembling...
MASM PROGRAM.ASM; > PROGRAM.LOG
if not exist PROGRAM.OBJ exit

echo Linking...
LINK PROGRAM.OBJ; >> PROGRAM.LOG
if not exist PROGRAM.EXE exit

echo Running...
echo on
PROGRAM.EXE
