@echo off
setlocal EnableDelayedExpansion
rem @rem 字符串及图片编译
rem cd ..\Logic\hzk
rem if exist .\GetFontB.bat call .\GetFontB.bat
rem if exist .\GetStr.bat call .\GetStr.bat
rem cd ..\..\MDK-ARM

set filename=X1.HL080A.K05.001-1_V100AXX

rem 修改微码名称和版本号工具使用的参数

rem 深度，User.bat与01OUT文件夹的层次差
set re_depth="2"

rem 是否修改除除烧录文件外的其它归档资料
set re_rename_other="0"

rem 是否更新归档资料微码名称
set re_update_name="1"

rem -path User.bat及bin文件路径 -debug 使用debug模式输出更多信息0：不使用，1：使用

@rem 生成bin文件
hex2bin.exe  .\obj\*.hex
copy  .\obj\*.bin  .\work.bin


set OBJDIR=.\obj

rem 检查HEX文件是不是有且仅有1个
set dirsrc=%OBJDIR%\*.hex
set srchex=.\
set hexnum=0
for %%i in (%dirsrc%) do (
    set /a hexnum=!hexnum!+1
    set srchex=%%i
)

if %hexnum% NEQ 1 (goto HexError)

rem 检查OBJ中是否有SCT文件
if exist %OBJDIR%\*.sct (goto ObjError1)

rem 检查SCT文件只有1个
set dirsct=.\linker\*.sct
set srcsct=.\
set sctnum=0
for %%i in (%dirsct%) do (
    set /a sctnum=!sctnum!+1
    set srcsct=%%~ni
)

if %sctnum% NEQ 1 (goto ObjError2)


@rem 复制HEX文件进行加密
@copy /b .\obj\*.hex .\app.hex

echo 开始加密……
set ENTRYPT=0
if %srcsct% EQU s6e2h14_256KB            (set ENTRYPT=256)
if %srcsct% EQU s6e2h14_256KB_32KB       (set ENTRYPT=256)
if %srcsct% EQU s6e2h14_256KB_256KB      (set ENTRYPT=512)
if %srcsct% EQU s6e2h14_256KB_256KB_32KB (set ENTRYPT=512)
if %srcsct% EQU s6e2h14_256KB_256KB_64KB (set ENTRYPT=512)
if %srcsct% EQU s6e2h14_512KB            (set ENTRYPT=512)
if %srcsct% EQU s6e2h14_512KB_32KB       (set ENTRYPT=512)

if %ENTRYPT% EQU 0 (goto SctError)

if %ENTRYPT% EQU 256 (
echo 256KB加密方式
@..\Tool\PLD\PLDownloader.exe -h -v2 -f4 -a00000000 app.hex
goto Combine
) 

if %ENTRYPT% EQU 512 (
echo 512KB加密方式
@..\Tool\PLD\PLDownloader.exe -h -v2 -f8 -a00000000 app.hex
goto Combine
) 

:Combine
echo 整理应用和BOOT文件，用于生产烧录
@copy /b ..\Tool\Combine\*.hex		.\boot.hex
@..\Tool\Combine\CombineHex.exe "boot.hex" "app.hex"  "12.hex"
@copy /b ..\Tool\Combine\secure\secure.hex		.\secure.hex
@..\Tool\Combine\CombineHex.exe "12.hex" "secure.hex"  "work.hex"
@del .\boot.hex .\app.hex .\secure.hex
rem pause



echo 移动文件输出

if exist ..\..\01OUT\%filename%.plcx (
    del /Q ..\..\01OUT\%filename%.plcx
)
if exist ..\..\01OUT\%filename%.hex (
    del /Q ..\..\01OUT\%filename%.hex
)
PlcxCheck.exe app.plcx
PyAppendCRC32.exe app.plcx
move 12.hex   ..\Chip\%filename%.NoSecure.hex
move work.hex ..\..\01OUT\%filename%.hex
move app.plcx ..\..\01OUT\%filename%.plcx

@rem 修改微码名称和版本号
py_rename_version.exe -path="%cd%" -depth=%re_depth% -rename_other=%re_rename_other%  -update_name=%re_update_name% -debug="0"

if exist "C:\Documents and Settings\pp\My Documents\" (
copy .\obj\*.hex "C:\Documents and Settings\pp\My Documents\STM32.hex"
echo 复制到我的文档
)

if exist "D:\我的文档\" (
copy .\obj\*.hex "D:\我的文档\STM32.hex"
echo 复制到我的文档
)

goto EOF

:HexError
echo 错误：HEX文件不为1个。
goto EOF

:ObjError1
echo 错误：Obj中有sct文件，请使用自定义sct文件。
goto EOF

:ObjError2
echo 错误：linker中sct文件不为1个，请按要求使用sct文件。
goto EOF

:SctError
echo 错误：不支持的sct文件，请按要求使用sct文件。
goto EOF






:EOF


rem pause


rem pause > nul
