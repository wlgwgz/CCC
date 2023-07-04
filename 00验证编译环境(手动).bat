@echo off

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

git checkout -B verify-compile-env 0.0.3
echo 已切换至编译环境验证分支

echo.
echo. 
ping 127.0.0.1 -n 2 >nul

rem pause

cd .\MDK_ARM
FOR  %%a IN (*.uvprojx) DO (
	start /b %KEIL5_ARM%\UV4\UV4.exe %%a
)
echo 已打开Keil工程
echo.

echo ********************************************
echo 注意：请按以下步骤执行！！！！！！！！！
echo ********************************************
echo.

echo 步骤1：选择任意编译目标，并对当前编译目标执行全编译（Rebuild）
echo.
echo 步骤2：在Tool文件夹中，选取相应的 **编译环境基准文件**（如：编译目标为“DEBUG”时，**编译环境基准文件** = 编译环境基准_DEBUG(0.0.1).hex）
echo.
echo 步骤3：将 **编译环境基准文件** 与 **01OUT文件夹** 中的HEX文件进行对比，检查内容是否一致！
echo.

pause