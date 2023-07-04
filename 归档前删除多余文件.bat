rem @echo off
echo 正在删除多余文件...
echo. 

del MDK_ARM\*.bak /q
del MDK_ARM\JLinkLog.txt /q
rd MDK_ARM\LST /s /q
rd MDK_ARM\OBJ /s /q

rem del PJ\*.IAB /q
rem del PJ\*.PS /q

echo. 
echo 已删除所有多余文件！
echo. 
ping 127.0.0.1 -n 2 >nul

rem pause

