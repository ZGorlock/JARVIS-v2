@echo off
echo relaunching DLA...

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

echo launching DLA...
start DLA.exe

echo relaunched DLA!

exit
