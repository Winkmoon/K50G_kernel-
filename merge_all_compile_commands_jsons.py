import json
import os
from pathlib import Path

def merge_compile_commands(directory):
    merged_commands = []

    # 遍历目录中的所有compile_commands.json文件
    for file in Path(directory).glob('**/compile_commands.json'):
        with open(file, 'r') as f:
            # 将JSON内容添加到合并列表中
            commands = json.load(f)
            merged_commands.extend(commands)

    # 将合并后的列表写回到新的JSON文件中
    with open('merged_compile_commands.json', 'w') as f:
        json.dump(merged_commands, f, indent=4)


# 使用当前脚本的目录作为默认目录
current_directory = os.getcwd()
merge_compile_commands(current_directory)
