@echo off

cd .\MDK_ARM
FOR  %%a IN (*.uvprojx) DO (
	start /b %KEIL5_ARM%\UV4\UV4.exe %%a
)

