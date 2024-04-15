echo off

if EXIST "%NOMAD_HOME_PERSONAL%" cd "%NOMAD_HOME_PERSONAL%\examples\basic\batch\single_obj"

echo.
echo ********************************************************
echo Test write permission in the current directory
echo ********************************************************
echo Test write permission in the current directory > testWritePermission.txt
IF EXIST "testWritePermission.txt" (echo Write permission granted. You can run Nomad in this directory) ELSE (echo Write permission not granted. You cannot use Nomad in this directory for solving problems.)
IF EXIST "testWritePermission.txt" del testWritePermission.txt


echo.
if NOT EXIST "%NOMAD_HOME_PERSONAL%" echo ********************************************************
if NOT EXIST "%NOMAD_HOME_PERSONAL%" echo Nomad examples can be copied 
if NOT EXIST "%NOMAD_HOME_PERSONAL%" echo from %NOMAD_HOME%\examples
if NOT EXIST "%NOMAD_HOME_PERSONAL%" echo ********************************************************


echo.
echo ********************************************************
echo Output obtained by running the command 'nomad.exe -info'
echo ********************************************************
cmd /K "%NOMAD_HOME%\bin\nomad.exe" -info
echo.

