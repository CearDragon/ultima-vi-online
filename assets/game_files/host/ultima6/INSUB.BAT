echo off
:adv1
:adv3
cls
if exist %2\u6.4 goto adv3a
echo Please insert the GAME disk.
pause
goto adv3
:adv3a
cls
echo Copying files...
copy %2\*.*>NUL
cls
:adv7
cls
install
