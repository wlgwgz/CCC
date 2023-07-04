#!/bin/bash
# -*- coding: utf-8 -*-

echo 
echo "******************************************************************"
echo "PyCombineDoc启动器.sh"
echo 
echo "**注意事项**"
echo "1. 脚本功能：辅助PyCinbubeDoc传参" 
echo 
echo "Author: lzf@punp.com"
echo "Date: 2023-05-05"
echo "******************************************************************"

echo
echo "已支持以下目标："
echo "<0> 全部"
echo "<1> 中山TCL"
echo "<2> 中山长虹"
echo "<3> 通用"
echo "<4> 广州迪森"
echo "<5> 广东派沃"
echo "<6> 广东纽恩泰"
echo "<7> 浙江生能"
echo "<8> 佛山鸿禾"
echo "<9> 万和集团"
echo "<10> 广东长帆"
read -p "1. 请输入目标编号（xx）：" targetId

case $targetId in
    0)
    targetCode=ALL
    ;;

    1)
    targetCode=TCL
    ;;
    
    2)
    targetCode=CHANGHONG
    ;;
    
    3)
    targetCode=TONGYONG
    ;;
    
    4)
    targetCode=DISEN
    ;;
    
    5)
    targetCode=PAIWO
    ;;
    
    6)
    targetCode=NIUENTAI
    ;;
    
    7)
    targetCode=SHENGNENG
    ;;
    
    8)
    targetCode=HONGHE
    ;;
    
    9)
    targetCode=WANHE
    ;;
    
    10)
    targetCode=CHANGFAN
    ;;

    *)
    echo "错误：目标未知！"
    read -t3
    exit
    ;;
esac

echo
echo "已支持以下类型："
echo "<1> 内外机规格书"
echo "<2> 外机规格书"

read -p "2. 请输入目标类型（xx）：" targetId

case $targetId in
    1)
    targetType=idu-odu
    ;;
    
    2)
    targetType=odu
    ;;
    
    *)
    echo "错误：类型未知！"
    read -t3
    exit
    ;;
esac

targetDirName="custom-${targetCode}-${targetType}"

workPath="$(dirname "$0")\fsd"

para[0]="-work_path=\"${workPath}\""
para[1]="-output_path=\"${workPath}\__OUTPUT_TMP__\""
para[2]="-config_item_name=${targetType}-${targetCode}"
para[3]="-config_path=\"${workPath}\config.json\""

if [ -e '.\PyCombineDoc.exe' ]; 
then
    .\\PyCombineDoc.exe "${para[0]}" "${para[1]}" "${para[2]}" "${para[3]}"
else
    PyCombineDoc.exe "${para[0]}" "${para[1]}" "${para[2]}" "${para[3]}"
fi

echo

echo 删除原来的文件夹...
rm -r "${workPath}\\__OUTPUT_TMP__\\${targetDirName}"
echo 完成
echo

echo 移动并重命名输出文件夹...
mv "${workPath}\__OUTPUT_TMP__\output" "${workPath}\\__OUTPUT_TMP__\\${targetDirName}"
echo 完成
echo

# echo 删除临时文件夹...
# rm -r "${workPath}\__OUTPUT_TMP__"
# echo 完成
# echo

read -s -n1 -p "按任意键结束..."