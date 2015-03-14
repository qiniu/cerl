@echo off
if "%1" == "" (
	echo "Usage: venusc <server_description_language_file.sdl>"
	goto exit
	)
set AppPath=%~dp0
"%AppPath%\sdl2json.exe" %1 | php "%AppPath%\gen_cpps"
set AppPath=
:exit
