@echo off

echo ɾ���ɹ����ļ�...
echo.

del MDK_ARM\*.uvopt /q
del MDK_ARM\*.uvoptx /q
del MDK_ARM\*.uvprojx /q

rd PJ /s /q

echo. 
echo ��ɾ�����оɹ����ļ���

echo ��ѹѹ����...
echo.
if exist .\%folde% (
"C:\Program Files\WinRAR\WinRAR.exe" x Chip^&PJ.zip .\%folde%)

echo ��ѹ��ɣ�
echo.
echo. 
ping 127.0.0.1 -n 2 >nul

rem pause

git checkout -B verify-compile-env 0.0.3
echo ���л������뻷����֤��֧

echo.
echo. 
ping 127.0.0.1 -n 2 >nul

rem pause

cd .\MDK_ARM
FOR  %%a IN (*.uvprojx) DO (
	start /b %KEIL5_ARM%\UV4\UV4.exe %%a
)
echo �Ѵ�Keil����
echo.

echo ********************************************
echo ע�⣺�밴���²���ִ�У�����������������
echo ********************************************
echo.

echo ����1��ѡ���������Ŀ�꣬���Ե�ǰ����Ŀ��ִ��ȫ���루Rebuild��
echo.
echo ����2����Tool�ļ����У�ѡȡ��Ӧ�� **���뻷����׼�ļ�**���磺����Ŀ��Ϊ��DEBUG��ʱ��**���뻷����׼�ļ�** = ���뻷����׼_DEBUG(0.0.1).hex��
echo.
echo ����3���� **���뻷����׼�ļ�** �� **01OUT�ļ���** �е�HEX�ļ����жԱȣ���������Ƿ�һ�£�
echo.

pause