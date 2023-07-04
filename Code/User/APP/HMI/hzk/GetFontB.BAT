rem autorun
rem 第一行为“rem autorun”表示运行“GetStr.bat”后会自动调用本文件
rem 如果不想自动调用，如后板空间不够，则修改第一行。

@echo off
rem GetFontB_V200.exe AACH16.c AABMP.c [起始汉字] [结束汉字(不含)] [起始图片] [结束图片(不含)] AACH_B.c AACH_B.h
GetFontB_V200.exe AACH16.c AABMP.c a527 a54a f535 f535 AACH_B.c AACH_B.h

rem ping -n 2 127.0.0.1 >NUL
rem pause
