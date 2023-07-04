@echo off

set DIR=%cd%\Code\User\APP\HMI\hzk

FOR /r "%DIR%" %%a IN (*.csv) DO (
	start /b "" "%%a"
)