#!/bin/bash

srcPath="/home/ubuntu/workdir/app/ota_save"
desPath="/home/ubuntu/workdir/app"
AppName="WeighBox"
UpdaterName="WeighBoxUpdater"
fileList=("WeighBox")

# 创建日志文件
logFile="/home/ubuntu/workdir/app/ota_log.log"
touch $logFile

# 检查源文件路径是否存在
if [ -d "$srcPath" ]; then
  if [ -f "$desPath/$AppName" ]; then
    echo "$AppName 文件存在，杀死$AppName进程" >> $logFile
    pkill -x $AppName
    echo "Waiting..." >> $logFile
    sleep 1
    echo "关闭看门狗..." >> $logFile
    echo V > /dev/watchdog
  fi

  # 源文件路径存在，开始拷贝操作
  for file in "${fileList[@]}"; do
    srcFile="$srcPath/$file"
    if [ -f "$srcFile" ]; then
      mv -f "$srcFile" "$desPath"
      echo "拷贝文件: $file" >> $logFile
    else
      echo "$srcFile 不存在" >> $logFile
    fi
  done

  if [ $? -eq 0 ]; then
    echo "文件拷贝成功" >> $logFile
    sleep 3
    if [ -f "$desPath/$AppName" ]; then
      echo "$AppName 文件存在，启动$AppName" >> $logFile
      chmod 777 "$desPath/$AppName"
    else
      echo "$AppName 文件不存在" >> $logFile
    fi
  else
    echo "文件拷贝失败" >> $logFile
  fi
else
  echo "源文件路径不存在" >> $logFile
fi
