@echo off
echo backing up DLA...

echo getting DLA location...
set /p loc= 0<"%ProgramData%\DLA\loc"
if "%loc%" == "" (
    echo Install DLA first! Closing in 5 seconds...
    @ping 127.0.0.1 -n 5 -w 1000> nul
    exit
)
cd "%loc%"

echo getting timestamp...
for /f "tokens=1-4 delims=/ " %%a in ("%date%") do (
    set day=%%c&set month=%%b&set year=%%d
)
for /f "tokens= 1-4 delims=:." %%a in ("%time%") do (
    set hour=%%a&set mins=%%b&set secs=%%c
)
if %hour% lss 10 set hour=0%hour: =%
set timestamp=%year%%month%%day%%hour%%mins%%secs%

echo making backup directory...
cd bin\backup
mkdir %timestamp%
cd %timestamp%
mkdir compile
mkdir project
mkdir source
mkdir data
cd compile
mkdir DLA.tlog
cd ..\..\..\..

echo backing up development files...
xcopy /c /q /r /y DLA.exe.manifest bin\backup\%timestamp%\compile\* >nul 2>&1
REM xcopy /c /q /r /y DLA.pdb bin\backup\%timestamp%\compile\* >nul 2>&1
xcopy /c /q /r /y DLA.exe bin\backup\%timestamp%\project\* >nul 2>&1
xcopy /c /q /r /y DLA.sln bin\backup\%timestamp%\project\* >nul 2>&1
REM xcopy /c /q /r /y DLA.sdf bin\backup\%timestamp%\project\* >nul 2>&1
xcopy /c /q /r /y DLA.vcxproj bin\backup\%timestamp%\project\* >nul 2>&1
xcopy /c /q /r /y DLA.vcxproj.filters bin\backup\%timestamp%\project\* >nul 2>&1
xcopy /c /q /r /y DLA.vcxproj.user bin\backup\%timestamp%\project\* >nul 2>&1
xcopy /c /q /r /y DLA.v12.suo bin\backup\%timestamp%\project\* >nul 2>&1

echo backing up other files...
xcopy /c /i /s /q /r /y source\*.* bin\backup\%timestamp%\source\* >nul 2>&1
xcopy /c /i /s /q /r /y data\*.* bin\backup\%timestamp%\data\* >nul 2>&1

echo compressing backup...
bin\batch\zip.bat bin\backup ..\..\ %timestamp%.zip %timestamp% 1

echo backed up DLA!

exit