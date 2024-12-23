#!/bin/bash

current_date_time=$(date)
git add * 
git commit -m "同步_$current_date_time"

git push 

echo "同步完成_本次同步时间: $custom_format"
