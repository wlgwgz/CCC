@echo off
echo. V1.01-zzp
echo. 

rem ΢�����ƣ��������Ҫ�޸ģ�����
set x1code=X1.HL080A.K05.001-1

rem ��ǰ�ļ�������
for /f "delims=" %%i in ("%cd%") do set folde=%%~nxi

if exist .\�鵵ǰɾ�������ļ�.bat (
    echo.ɾ�������ļ�...
    echo.
    call �鵵ǰɾ�������ļ�.bat
)

cd ..
set rarname=%x1code%.SourceCode

echo �ļ�����%rarname%
echo.

echo ɾ�����ļ�...
echo.
if exist %rarname%.rar (
   del %rarname%.rar
)

rem ��ʡ�ռ䣬���鵵SI�ļ���.git�ļ��С�Docs�ļ��С�.gitlab�ļ��У���Сѹ������С��ѹ��ʱ���ԣ�����
echo ����ѹ����...
echo.
if exist .\%folde% (
"C:\Program Files\WinRAR\WinRAR.exe" a -r -m3 -t -x*\PJ -x*\.git -x*\Docs -x*\.gitlab %rarname%.rar .\%folde%)

echo ������ɣ�
echo.

ping 127.0.0.1 -n 2 >nul
echo.