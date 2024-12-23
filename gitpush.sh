#!/bin/bash

echo "欢迎使用TS同步脚本，下面开始进行文件同步到云"

git pull

current_date_time=$(date)
git add * 
git commit -m "同步_$current_date_time"

git push 

echo "同步完成_本次同步时间: $current_date_time"
