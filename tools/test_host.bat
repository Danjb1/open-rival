cd ..\build\windows-vs2022-x64\projects\Open-Rival

:: Make a copy so that we don't lock the file.
:: Otherwise we can't run it through Visual Studio at the same time!
copy Debug\Open-Rival.exe Debug\Open-Rival.copy.exe

Debug\Open-Rival.copy.exe -host -pos 150,300
pause
