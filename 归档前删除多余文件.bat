rem @echo off
echo ����ɾ�������ļ�...
echo. 

del MDK_ARM\*.bak /q
del MDK_ARM\JLinkLog.txt /q
rd MDK_ARM\LST /s /q
rd MDK_ARM\OBJ /s /q

rem del PJ\*.IAB /q
rem del PJ\*.PS /q

echo. 
echo ��ɾ�����ж����ļ���
echo. 
ping 127.0.0.1 -n 2 >nul

rem pause

