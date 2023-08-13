@echo off
setlocal

REM Set up the Visual Studio build environment
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64

REM Set paths for includes and libraries
set INCLUDE_PATH=include
set LIB_PATH=lib

REM Compile the C files to object files
cl /c /W4 /Zi /I "%INCLUDE_PATH%" tools\encrypt_config.c /Foencrypt_config.obj
cl /c /W4 /Zi /I "%INCLUDE_PATH%" tools\validate_config.c /Fovalidate_config.obj

cl /c /W4 /Zi /I "%INCLUDE_PATH%" src\config\config.c /Fosrc\config\config.obj
cl /c /W4 /Zi /I "%INCLUDE_PATH%" src\util\crypto.c /Fosrc\util\crypto.obj

REM Link the object files to create the executable
link /LIBPATH:"%LIB_PATH%" /OUT:tools\encrypt_config.exe encrypt_config.obj src\config\config.obj src\util\crypto.obj crypt32.lib
link /LIBPATH:"%LIB_PATH%" /OUT:tools\validate_config.exe validate_config.obj src\config\config.obj src\util\crypto.obj crypt32.lib

REM Cleanup step
del encrypt_config.obj
del validate_config.obj
del src\config\config.obj
del src\util\crypto.obj

endlocal
pause
