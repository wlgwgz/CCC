#!/bin/bash

echo 
echo "******************************************************************"
echo "追加软件更改说明.sh"
echo 
echo "**注意事项**"
echo "1. 脚本功能：将提交信息导出至 “xxx.软件更改说明.csv”" 
echo "2. 提交信息导出完成后，切记调整 “xxx.软件更改说明.csv” 中的内容！"
echo "3. 请使用 VSCode + Edit csv插件 来编辑 “xxx.软件更改说明.csv”"
echo "4. 严禁使用 Excel 对 “xxx.软件更改说明.csv” 进行直接编辑和保存"
echo 
echo "Author: hj@punp.com"
echo "Date: 2023-04-24"
echo "******************************************************************"

echo
echo "已支持以下目标："
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

declare -x targetCode targetName appCode logHeader logFormat grepInfo grepOption
case $targetId in
  0)
     targetCode="DEBUG"
     targetName="DEBUG"
     appCode="X1.HL080A.K05.000-1"
     logHeader="正式版本,临时版本,日期,更改内容"
     logFormat="请填写正式版本,请填写临时版本,%as,%s"
     grepInfo=""
     ;;
  1)
     targetCode="TCL"
     targetName="中山TCL"
     appCode="X1.HL080A.K05.001-1"
     logHeader="正式版本,临时版本,日期,更改内容"
     logFormat="请填写正式版本,请填写临时版本,%as,%s"
     grepInfo="ALL|TCL"
     ;;
  2)
     targetCode="CHANGHONG"
     targetName="中山长虹"
     appCode="X1.HL080A.K05.501-1"
     logHeader="正式版本,临时版本,日期,更改内容"
     logFormat="请填写正式版本(若无则用"/"),请填写临时版本(若无则用"/"),%as,%s"
     grepInfo="ALL|CHANGHONG"
     ;;
  3)
     targetCode="TONGYONG"
     targetName="通用"
     appCode="X1.HL087A.K05.001-1"
     logHeader="正式版本,临时版本,日期,更改内容"
     logFormat="请填写正式版本(若无则用"/"),请填写临时版本(若无则用"/"),%as,%s---%b"
     grepInfo="ALL|TONGYONG"
     ;;
  4)
     targetCode="DISEN"
     targetName="广州迪森"
     appCode="X1.HL087A.K05.502-1"
     logHeader="正式版本,临时版本,日期,更改内容"
     logFormat="请填写正式版本(若无则用"/"),请填写临时版本(若无则用"/"),%as,%s---%b"
     grepInfo="ALL|DISEN"
     ;;
  5)
     targetCode="PAIWO"
     targetName="广东派沃"
     appCode="X1.HL087A.K05.501-1"
     logHeader="正式版本,临时版本,日期,更改内容"
     logFormat="请填写正式版本(若无则用"/"),请填写临时版本(若无则用"/"),%as,%s"
     grepInfo="ALL|PAIWO"
     ;;
  6)
     targetCode="NIUENTAI"
     targetName="广东纽恩泰"
     appCode="X1.HL087A.K05.503-1"
     logHeader="正式版本,临时版本,修改人,更改需求、问题及现象,原因分析,更改内容,测试方案,邦普验证时间,邦普验证负责人"
     logFormat="请填写正式版本(若无则用"/"),请填写临时版本(若无则用"/"),请填写修改人,请填写更改需求、问题及现象(若无则用"/"),请填写原因分析(若无则用"/"),%s---%b,请填写测试方案(若无则用"/"),请填写邦普验证时间,请填写邦普验证负责人"
     grepInfo="ALL|NIUENTAI"
     ;;
  7)
     targetCode="SHENGNENG"
     targetName="浙江生能"
     appCode="X1.HL087A.K05.504-1"
     logHeader="正式版本,临时版本,日期,更改内容"
     logFormat="请填写正式版本(若无则用"/"),请填写临时版本(若无则用"/"),%as,%s---%b"
     grepInfo="ALL|SHENGNENG"
     ;;
  8)
     targetCode="HONGHE"
     targetName="佛山鸿禾"
     appCode="X1.HL087A.K05.505-1"
     logHeader="正式版本,临时版本,日期,更改内容"
     logFormat="请填写正式版本(若无则用"/"),请填写临时版本(若无则用"/"),%as,%s---%b"
     grepInfo="ALL|HONGHE"
     ;;
  9)
     targetCode="WANHE"
     targetName="万和集团"
     appCode="X1.HL087A.K05.506-1"
     logHeader="正式版本,临时版本,日期,更改内容"
     logFormat="请填写正式版本(若无则用"/"),请填写临时版本(若无则用"/"),%as,%s---%b"
     grepInfo="ALL|WANHE"
     ;;
  10)
     targetCode="CHANGFAN"
     targetName="广东长帆"
     appCode="X1.HL087A.K05.507-1"
     logHeader="正式版本,临时版本,日期,更改内容"
     logFormat="请填写正式版本(若无则用"/"),请填写临时版本(若无则用"/"),%as,%s---%b"
     grepInfo="ALL|CHANGFAN"
     ;;
  *) 
     echo "错误：目标未知！"
     read -t3
     exit
     ;;
esac
grepOption="--grep=$grepInfo -E"


echo
read -p "2. 请输入日志导出起始日期[YYYY-MM-DD]: " dateBgn

echo
read -p "3. 请输入日志导出结束日期[YYYY-MM-DD]: " dateEnd

declare -x dateOption
if [ -n "$dateBgn" -a -n "$dateEnd" ]; then
  dateOption="--since=$dateBgn --until=$dateEnd"
elif [ -n "$dateBgn" -a -z "$dateEnd" ]; then
  dateOption="--since=$dateBgn"
else
  dateOption=""
fi

echo
echo "提交筛选(可选)："
echo "<1> 显示所有合并请求提交"
echo "<2> 显示所有非合并请求提交"
echo "<3> 仅显示实际生效的合并请求提交"
read -p "4. 请选择提交筛选：" filterOption

declare -x mergeOption
case $filterOption in
  1)
    mergeOption="--merges"
    grepOption=""
    ;;
  2)
    mergeOption="--no-merges"
    grepOption=""
    ;;
  3)
    mergeOption="--merges"
    ;;
  *)
    mergeOption=""
    grepOption=""
    ;;
esac

echo
echo $targetName: $appCode
if [ $filterOption = 3 ]; then
  echo "自动匹配：$grepInfo"
else
  echo "不使用自动匹配"
fi

if [ -n "$dateEnd" ]; then
  read -p "导出从 $dateBgn 到 $dateEnd 的提交信息？[Y/N]" ans
elif [ -n "$dateBgn" ]; then
  read -p "导出从 $dateBgn 至今的提交信息？[Y/N]" ans
else
  read -p "导出所有提交信息？[Y/N]" ans
fi

targetDir=log/$targetCode
targetFile=$targetDir/$appCode.软件更改记录.csv

if [ $ans = Y -o $ans = y ]; then
  #判断路径是否存在
  if [ -d "$targetDir" ]; then
    echo "文件路径已存在！"
  else
    mkdir "$targetDir" 
  fi

  #判断文件是否存在
  if [ -f "$targetFile" ]; then
    echo "文件已存在！从尾部追加记录！"
  else
    echo "文件不存在！创建文件！"
    #写入UTF-8 BOM
    printf '\xEF\xBB\xBF' > $targetFile
    printf $logHeader'\r\n' >> $targetFile
  fi

  # 配置编码类型
  git config i18n.commitencoding utf-8
  git config i18n.logoutputencoding utf-8
  
  #将规定格式输出至目标文件
  git log --reverse $mergeOption --pretty=format:$logFormat $dateOption $grepOption >> $targetFile
  printf '\r\n' >> $targetFile

  #UNIX行尾转换为Windows行尾
  #sed -ri 's/(.)$/\1\r/' $targetFile
fi
read -t5 