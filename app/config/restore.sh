#!/bin/bash

srcPath="/home/ubuntu/workdir/app/ota_backup"
desPath="/home/ubuntu/workdir/app"
AppName="WeighBox"
UpdaterName="WeighBoxUpdater"
# fileList=("WeighBox" "OtaUpdater" "sensor.ini")
fileList=("WeighBox")

# 检查源文件路径是否存在
if [ -d "$srcPath" ]; then
  # 检查目标文件中的$AppName文件是否存在
  if [ -f "$desPath/$AppName" ]; then
    echo "$AppName 文件存在，杀死$AppName进程"
    pkill -x $AppName
    # pkill -f $UpdaterName
    echo "Waiting..."
    sleep 1
  fi

# 源文件路径存在，开始拷贝操作
  for file in "${fileList[@]}"; do
    srcFile="$srcPath/$file"
    if [ -f "$srcFile" ]; then
      # 如果是fileList中的文件，则进行拷贝并打印文件名
      cp -f "$srcFile" "$desPath"
      echo "拷贝文件: $file"
    else
      echo "$srcFile 不存在"
    fi
  done

  # 检查拷贝是否成功
  if [ $? -eq 0 ]; then
    echo "文件拷贝成功"

    # 检查目标文件中的$AppName文件是否存在
    if [ -f "$desPath/$AppName" ]; then
      echo "$AppName 文件存在，启动$AppName"

      # 给文件权限777
      chmod 777 "$desPath/$UpdaterName"
      chmod 777 "$desPath/$AppName"
      "$desPath/$AppName" &
      # sleep 3
      # echo "启动$UpdaterName"
      # "$desPath/$UpdaterName" &
    else
      echo "$AppName 文件不存在"
    fi
  else
    echo "文件拷贝失败"
  fi
else
  echo "源文件路径不存在"
fi
