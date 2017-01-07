@echo off
setlocal

echo ====================
echo  TWC -- Build Batch
echo ====================
echo.

set DEFINES= 
set CCFLAGS= /MT %DEFINES% /Gm- /WX /W4 /wd4201 /wd4100 /wd4189 /wd4505 /wd4706 /wd4996 /wd4127 /wd4244 /wd4200 /wd4204
set LIBS= libcurl.lib 

IF NOT EXIST build mkdir build
pushd build

cl /nologo /I../include %CCFLAGS% ../code/json.c ../code/codegen.c /link /incremental:no /opt:ref /out:twc_codegen.exe /subsystem:console,5.2
call twc_codegen.exe ../code/twitter.c ../api.json ../code/twitter_api.h ../code/twitter_api.c

IF NOT EXIST win64 mkdir win64

cl /nologo /c /I../include %CCFLAGS% ../code/twitter.c
lib /nologo twitter.obj /out:win64/libtwc.lib

cp ../code/twitter_api.h ../code/twitter.h ../code/types.h ../include
cp win64/libtwc.lib ../lib/win64/libtwc.lib

popd

:: IF NOT EXIST win32 mkdir win32
:: pushd win32
::     cl /nologo %CCFLAGS% ../../code/twitter.c ../../code/json.c ../../code/bo_windows.cpp /link %LIBS% %LDFLAGS% /LIBPATH:../../lib/win32 /OUT:%EXE_NAME% /subsystem:console,5.1 
::     if ERRORLEVEL 1 goto :eof
:: popd
:: echo.

