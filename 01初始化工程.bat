@echo off

echo ��ӽű���Ч
xcopy /y /s ".\.gitlab\hooks" ".\.git\hooks"
git-bash -c "chmod +x .git/hooks/pre-commit"
echo.

echo ������Ŀ�ύģ��
echo.
git config commit.template .gitlab/commit_templates/commit_template.md

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



