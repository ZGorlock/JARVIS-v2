@echo off
echo cleaning DLA directory...

echo get DLA location...
set /p loc= 0<"%ProgramData%\DLA\loc"
if "%loc%" == "" (
    echo Install DLA first! Closing in 5 seconds...
    @ping 127.0.0.1 -n 5 -w 1000> nul
    exit
)
cd "%loc%"

echo removing unecessary development files...
REM rmdir /q /s DLA.tlog >nul 2>&1
REM del /f /q *.obj >nul 2>&1
REM del /f /q DLA.log >nul 2>&1

echo emptying temp directory...
rmdir /q /s temp >nul 2>&1
mkdir temp >nul 2>&1

echo cleaned DLA directory!

exit