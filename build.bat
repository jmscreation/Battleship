@echo off
REM		Build Script

REM Set Compiler Settings Here

set CPP=c++
set GPP=g++
set OUTPUT=program.exe
set DEBUGMODE=1
set FLAGS=-I. -Isrc -Icompleted -Ilibrary\SFML-2.5.1\include -g -std=c++20 -DSFML_STATIC
set LINKING=-Llibrary\SFML-2.5.1\lib64 -static -lsfml-network-s -lsfml-system-s -lwinmm -lws2_32 -static-libstdc++ -lpthread -lsetupapi -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs
set ASYNC_BUILD=1


del %OUTPUT% 2>nul

del /S /Q .objs64\*.o

setlocal enabledelayedexpansion

if not exist .objs64 (
	echo Creating Object Directory Structure...
	mkdir .objs64
)

if not exist completed (
	echo Creating Object Directory Structure...
	mkdir completed
)

if %ASYNC_BUILD% GTR 0 (
	set ASYNC=
) else (
	set ASYNC=/WAIT
)

echo Building Game Files...
for %%F in (src\*.cpp) do (
	if not exist .objs64\%%~nF.o (
		echo Building %%~nF.o
		start /B %ASYNC% "%%~nF.o" %CPP% %FLAGS% -c %%F -o .objs64\%%~nF.o
	)
)

for %%F in (completed\*.cpp) do (
	if not exist completed\%%~nF.o (
		echo Building %%~nF.o
		start /B %ASYNC% "%%~nF.o" %CPP% %FLAGS% -c %%F -o completed\%%~nF.o
	)
)

REM Wait for building process to finish
:loop
for /f %%G in ('tasklist ^| find /c "%CPP%"') do ( set count=%%G )
if %count%==0 (
	goto linker
) else (
	timeout /t 2 /nobreak>nul
	goto loop
)

:linker

set "files="
for /f "delims=" %%A in ('dir /b /a-d ".objs64\*.o" ') do set "files=!files! .objs64\%%A"
for /f "delims=" %%A in ('dir /b /a-d "completed\*.o" ') do set "files=!files! completed\%%A"

:link
echo Linking Executable...

if %DEBUGMODE% GTR 0 (
	set MWINDOWS=
) else (
	set MWINDOWS=-mwindows
)

%GPP% -o %OUTPUT% %files% %LINKING% %MWINDOWS%

:finish
if exist .\%OUTPUT% (
	echo Build Success!
) else (
	echo Build Failed!
)

%OUTPUT% host