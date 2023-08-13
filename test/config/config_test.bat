@echo off
setlocal

REM Set up the Visual Studio build environment
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64

REM Set paths for includes and libraries
set INCLUDE_PATH=include
set LIB_PATH=lib

REM Compile the C files to object files
cl /c /W4 /Zi /I "%INCLUDE_PATH%" test\config\config_test.c /Foconfig_test.obj
cl /c /W4 /Zi /I "%INCLUDE_PATH%" src\config\config.c /Fosrc\config\config.obj
cl /c /W4 /Zi /I "%INCLUDE_PATH%" src\util\crypto.c /Fosrc\util\crypto.obj

REM Link the object files to create the executable
link /LIBPATH:"%LIB_PATH%" /OUT:test\config_test.exe config_test.obj src\config\config.obj src\util\crypto.obj crypt32.lib

REM Cleanup obj
del src\config\config.obj
del src\util\crypto.obj
del config_test.obj

REM Run the test
test\config_test.exe
pause

REM Cleanup exe
del test\config_test.exe

endlocal
