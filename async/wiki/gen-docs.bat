@echo off
set AppPath=%~dp0
cd %AppPath%\..\..\docs\wwwroot
for %%i in (..\..\async\wiki\async\*.h) do (
	echo.
	echo Compiling %%i ...
	call cpp2htmls %%i
	)
set AppPath=
pause
