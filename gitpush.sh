#!/bin/bash

# 脚本版本
SCRIPT_VERSION="1.0.0"

# 日志文件路径
LOG_FILE="$(dirname "$0")/log"
MAX_LOG_SIZE=1048576 # 1MB

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # 恢复默认颜色

# 显示帮助信息
show_help() {
    echo -e "${GREEN}用法: $0 [-m 提交信息]${NC}"
    echo -e "${GREEN}选项:${NC}"
    echo -e "${GREEN}  -m 提交信息  添加自定义提交信息${NC}"
    echo -e "${GREEN}  -h           显示帮助信息${NC}"
    exit 0
}

# 日志函数
log() {
    echo -e "$(date '+%Y-%m-%d %H:%M:%S') - $1" | tee -a "$LOG_FILE"
}

# 检查日志文件大小
if [ -f "$LOG_FILE" ]; then
    log_size=$(wc -c < "$LOG_FILE")
    if [ $log_size -gt $MAX_LOG_SIZE ]; then
        log "${YELLOW}日志文件过大，清空日志...${NC}"
        > "$LOG_FILE"
    fi
fi

log "${GREEN}欢迎使用TS同步脚本 v$SCRIPT_VERSION${NC}"

# 检查参数
if [[ $1 == "-h" ]]; then
    show_help
elif [[ $1 == "-m" && -z $2 ]]; then
    log "${RED}错误：使用 -m 参数时需要提供提交信息。${NC}"
    exit 1
fi

# 检查当前分支
current_branch=$(git branch --show-current)
if [[ $current_branch != "main" ]]; then
    log "${YELLOW}警告：当前分支是 $current_branch，建议在 main 分支上操作。${NC}"
    read -p "是否继续？(y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# 开始同步
start_time=$(date +%s)

log "${GREEN}开始同步...${NC}"
git pull | tee -a "$LOG_FILE"
echo "" | tee -a "$LOG_FILE"
if [ $? -ne 0 ]; then
    log "${RED}错误：git pull 失败，请检查网络或仓库状态。${NC}"
    exit 1
fi

current_date_time=$(date)

if [[ $1 == "-m" && ! -z $2 ]]; then
    commit_message="$2_$current_date_time"
else
    commit_message="同步_$current_date_time"
fi

log "${GREEN}正在提交更改...${NC}"
git add * 
git commit -m "$commit_message" | tee -a "$LOG_FILE"
echo "" | tee -a "$LOG_FILE"

log "${GREEN}正在推送更改...${NC}"
git push | tee -a "$LOG_FILE"
echo "" | tee -a "$LOG_FILE"
if [ $? -ne 0 ]; then
    log "${RED}错误：git push 失败，请检查网络或仓库状态。${NC}"
    exit 1
fi

# 计算执行时间
end_time=$(date +%s)
execution_time=$((end_time - start_time))

log "${GREEN}同步完成_本次同步时间: $current_date_time${NC}"
log "${GREEN}脚本执行完成，总耗时: ${execution_time} 秒${NC}"

# 等待用户输入任意键结束
read -n 1 -s -r -p "按任意键结束..."
echo
