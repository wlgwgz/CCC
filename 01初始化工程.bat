@echo off

echo 令钩子脚本生效
xcopy /y /s ".\.gitlab\hooks" ".\.git\hooks"
git-bash -c "chmod +x .git/hooks/pre-commit"
echo.

echo 设置项目提交模板
echo.
git config commit.template .gitlab/commit_templates/commit_template.md

echo 删除旧工程文件...
echo.

del MDK_ARM\*.uvopt /q
del MDK_ARM\*.uvoptx /q
del MDK_ARM\*.uvprojx /q

rd PJ /s /q

echo. 
echo 已删除所有旧工程文件！

echo 解压压缩包...
echo.
if exist .\%folde% (
"C:\Program Files\WinRAR\WinRAR.exe" x Chip^&PJ.zip .\%folde%)

echo 解压完成！
echo.
echo. 
ping 127.0.0.1 -n 2 >nul

rem pause



