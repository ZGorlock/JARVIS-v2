@echo off
echo zipping folder...

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

echo zipping folder...
cd %1
%2resource\utilities\7-zip\7z.exe a -tzip -mx9 %3 %4 >%2temp\batch_zip_output_%timestamp%.log 2>&1

if %5==1 (
    echo deleting old folder...
    rmdir /q /s %4 >nul 2>&1
)

echo zipped folder!