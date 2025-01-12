#!/bin/bash

# 将脚本的输出同时显示在屏幕并写入 log 文件
exec > >(tee -a .log) 2>&1

echo "欢迎使用TS同步脚本，下面开始进行文件同步到云"

git pull

current_date_time=$(date)

# 检查是否带有 -m 参数
if [[ $1 == "-m" && ! -z $2 ]]; then
    commit_message="$2_$current_date_time"
else
    commit_message="同步_$current_date_time"
fi

git add * 
git commit -m "$commit_message"

git push 

echo "同步完成_本次同步时间: $current_date_time"

# 等待用户输入任意键结束
read -n 1 -s -r -p "脚本执行完成，按任意键结束..."
echo
