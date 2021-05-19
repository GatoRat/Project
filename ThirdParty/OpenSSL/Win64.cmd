@echo off
setlocal

set ORG_DIR=%~dp0
rem goto copyFiles

call "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
set PATH=%CD%;%PATH%
if exist Build rmdir /s /q Build
md Build
cd Build
set BUILD_DIR=%CD%

rem Build the library.
perl ..\src\Configure VC-WIN64A --api=1.1.0 no-ui no-idea no-md4 no-rc2 no-rc4 no-ssl3 no-camellia

if exist makefile.org del makefile.org
ren makefile makefile.org
perl -p ^
	-e "s/-Gs0/\/GS- \/GR- \/GL/;" ^
	-e "s/-Gy//;" ^
	-e "s/-GF//;" ^
	-e "s/\/opt:ref/\/opt:ref \/opt:icf \/ltcg/;" ^
	makefile.org > makefile

nmake -f makefile

:copyFiles
cd %ORG_DIR%

if not exist bin md bin
if not exist lib md lib

copy /y Build\libcrypto.lib         lib\libcrypto.lib
copy /y Build\libcrypto-1_1-x64.dll bin\libcrypto-1_1-x64.dll

copy /y Build\libssl.lib         lib\libssl.lib
copy /y Build\libssl-1_1-x64.dll bin\libssl-1_1-x64.dll

robocopy src\include\openssl openssl /s /purge /xo /fft /xa:SHTO /xf opensslconf.h
copy /y Build\include\openssl\opensslconf.h openssl\opensslconf.h

echo.

:end
endlocal
