@echo off
setlocal enabledelayedexpansion
PyUnicode appstr.csv
git-bash -c "md5sum appstr.csv | awk '{print $1}' > appstr_md5"

if exist str_version.h (
    (for /f tokens^=1^,2^,3^,4^,5*^ delims^=^" %%i in (str_version.h) do (
        if "%%j"=="" (
            echo.%%i
        ) else if 1==1  (
            for /f "tokens=1,2,3,4,5 delims=." %%a in ("%%k") do (
                set "ver=%%a"
                set /a num=1!ver!+1
                set "new_ver=%%i""%%j".!num:~-6!""
                echo.!new_ver!
            )
        )
    ))>temp.h
    
    if exist temp.h (
        del str_version.h
        ren temp.h str_version.h 
        echo 字符串版本升级成功！
    ) else if 1==1  (
        echo 字符串版本升级失败！
    )
) else if 1==1 (
    echo 未找到str_version.h文件！
)
echo.
echo 字符串编译成功！
echo.