@echo off
echo compiling DLA...

echo getting DLA location...
set /p loc= 0<"%ProgramData%\DLA\loc"
if "%loc%" == "" (
    echo Install DLA first! Closing in 5 seconds...
    @ping 127.0.0.1 -n 5 -w 1000> nul
    exit
)
cd "%loc%"

echo waiting for DLA to close...
set waiting=0
:while1
set /p inuse= 0<"bin\inuse"
if %inuse%==1 (
    set waiting=1
    @ping 127.0.0.1 -n 1 -w 1000> nul
    goto :while1
)
if %waiting%==1 (
    @ping 127.0.0.1 -n 2 -w 1000> nul
)
echo DLA is closed!

echo getting computer architecture...
if not %PROCESSOR_ARCHITECTURE%==x86 (
    set arch=x64
) else (
    set arch=x86
)

echo compiling DLA...
if %arch%==x64 (
    set msBuildDir=%WINDIR%\Microsoft.NET\Framework64\v4.0.30319
) else (
    set msBuildDir=%WINDIR%\Microsoft.NET\Framework\v4.0.30319
)
call "%msBuildDir%\msbuild.exe"  "%loc%DLA.vcxproj" /p:Configuration=Debug /l:FileLogger,Microsoft.Build.Engine;logfile=DLA.log >temp\batch_compile_debug_output_%timestamp%.log 2>&1

echo compiled DLA!

exit