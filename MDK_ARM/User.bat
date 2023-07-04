@echo off
setlocal EnableDelayedExpansion
rem @rem �ַ�����ͼƬ����
rem cd ..\Logic\hzk
rem if exist .\GetFontB.bat call .\GetFontB.bat
rem if exist .\GetStr.bat call .\GetStr.bat
rem cd ..\..\MDK-ARM

set filename=X1.HL080A.K05.001-1_V100AXX

rem �޸�΢�����ƺͰ汾�Ź���ʹ�õĲ���

rem ��ȣ�User.bat��01OUT�ļ��еĲ�β�
set re_depth="2"

rem �Ƿ��޸ĳ�����¼�ļ���������鵵����
set re_rename_other="0"

rem �Ƿ���¹鵵����΢������
set re_update_name="1"

rem -path User.bat��bin�ļ�·�� -debug ʹ��debugģʽ���������Ϣ0����ʹ�ã�1��ʹ��

@rem ����bin�ļ�
hex2bin.exe  .\obj\*.hex
copy  .\obj\*.bin  .\work.bin


set OBJDIR=.\obj

rem ���HEX�ļ��ǲ������ҽ���1��
set dirsrc=%OBJDIR%\*.hex
set srchex=.\
set hexnum=0
for %%i in (%dirsrc%) do (
    set /a hexnum=!hexnum!+1
    set srchex=%%i
)

if %hexnum% NEQ 1 (goto HexError)

rem ���OBJ���Ƿ���SCT�ļ�
if exist %OBJDIR%\*.sct (goto ObjError1)

rem ���SCT�ļ�ֻ��1��
set dirsct=.\linker\*.sct
set srcsct=.\
set sctnum=0
for %%i in (%dirsct%) do (
    set /a sctnum=!sctnum!+1
    set srcsct=%%~ni
)

if %sctnum% NEQ 1 (goto ObjError2)


@rem ����HEX�ļ����м���
@copy /b .\obj\*.hex .\app.hex

echo ��ʼ���ܡ���
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
echo 256KB���ܷ�ʽ
@..\Tool\PLD\PLDownloader.exe -h -v2 -f4 -a00000000 app.hex
goto Combine
) 

if %ENTRYPT% EQU 512 (
echo 512KB���ܷ�ʽ
@..\Tool\PLD\PLDownloader.exe -h -v2 -f8 -a00000000 app.hex
goto Combine
) 

:Combine
echo ����Ӧ�ú�BOOT�ļ�������������¼
@copy /b ..\Tool\Combine\*.hex		.\boot.hex
@..\Tool\Combine\CombineHex.exe "boot.hex" "app.hex"  "12.hex"
@copy /b ..\Tool\Combine\secure\secure.hex		.\secure.hex
@..\Tool\Combine\CombineHex.exe "12.hex" "secure.hex"  "work.hex"
@del .\boot.hex .\app.hex .\secure.hex
rem pause



echo �ƶ��ļ����

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

@rem �޸�΢�����ƺͰ汾��
py_rename_version.exe -path="%cd%" -depth=%re_depth% -rename_other=%re_rename_other%  -update_name=%re_update_name% -debug="0"

if exist "C:\Documents and Settings\pp\My Documents\" (
copy .\obj\*.hex "C:\Documents and Settings\pp\My Documents\STM32.hex"
echo ���Ƶ��ҵ��ĵ�
)

if exist "D:\�ҵ��ĵ�\" (
copy .\obj\*.hex "D:\�ҵ��ĵ�\STM32.hex"
echo ���Ƶ��ҵ��ĵ�
)

goto EOF

:HexError
echo ����HEX�ļ���Ϊ1����
goto EOF

:ObjError1
echo ����Obj����sct�ļ�����ʹ���Զ���sct�ļ���
goto EOF

:ObjError2
echo ����linker��sct�ļ���Ϊ1�����밴Ҫ��ʹ��sct�ļ���
goto EOF

:SctError
echo ���󣺲�֧�ֵ�sct�ļ����밴Ҫ��ʹ��sct�ļ���
goto EOF






:EOF


rem pause


rem pause > nul
