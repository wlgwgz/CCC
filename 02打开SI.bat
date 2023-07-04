@echo off

set DIR=%cd%\PJ

FOR /r "%DIR%" %%a IN (*.PR) DO (
	start /b "" "%%a"
)