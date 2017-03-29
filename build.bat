@echo off
setlocal

echo ====================
echo  TWC -- Build Batch
echo ====================
echo.

set DEFINES= 
set CCFLAGS= /MT %DEFINES% /Gm- /W4 /wd4201 /wd4100 /wd4189 /wd4505 /wd4706 /wd4996 /wd4127 /wd4244 /wd4200 /wd4204
set LIBS= libcurl.lib 

IF NOT EXIST build mkdir build
pushd build

cl /nologo /I../include %CCFLAGS% ../code/json.c ../code/codegen.c /link /incremental:no /opt:ref /out:twc_codegen.exe /subsystem:console,5.2
call twc_codegen.exe ../code/twitter.c ../api.json ../code/twitter_api.h ../code/twitter_api.c

cl /nologo /c /I../include %CCFLAGS% ../code/twitter.c

IF NOT EXIST win64 mkdir win64
lib /nologo twitter.obj /out:win64/libtwc.lib

:: IF NOT EXIST win32 mkdir win32
:: lib /nologo twitter.obj /out:win32/libtwc.lib

xcopy /Y ..\code\twitter_api.h ..\include
xcopy /Y ..\code\twitter.h ..\include
xcopy /Y win64\libtwc.lib ..\lib\win64\libtwc.lib

popd

