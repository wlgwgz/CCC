@echo off
echo. V1.01-zzp
echo. 

rem 微码名称，请根据需要修改！！！
set x1code=X1.HL080A.K05.001-1

rem 当前文件夹名称
for /f "delims=" %%i in ("%cd%") do set folde=%%~nxi

if exist .\归档前删除多余文件.bat (
    echo.删除多余文件...
    echo.
    call 归档前删除多余文件.bat
)

cd ..
set rarname=%x1code%.SourceCode

echo 文件名：%rarname%
echo.

echo 删除旧文件...
echo.
if exist %rarname%.rar (
   del %rarname%.rar
)

rem 节省空间，不归档SI文件、.git文件夹、Docs文件夹、.gitlab文件夹，减小压缩包大小：压缩时忽略！！！
echo 创建压缩包...
echo.
if exist .\%folde% (
"C:\Program Files\WinRAR\WinRAR.exe" a -r -m3 -t -x*\PJ -x*\.git -x*\Docs -x*\.gitlab %rarname%.rar .\%folde%)

echo 创建完成！
echo.

ping 127.0.0.1 -n 2 >nul
echo.