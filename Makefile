CC = g++                        # 使用 g++ 作为编译器
TARGET = bin/server.app          # 将生成的可执行文件放在 bin 目录下
SOURCE = $(wildcard ./src/*.cpp) # 获取 src 目录下的所有 .cpp 文件
OBJS = $(patsubst %.cpp, %.o, $(SOURCE))  # 将 .cpp 文件转换为 .o 对象文件
INCLUDE = -I./include            # 包含头文件目录
CFLAGS = -Wall -g                # 添加编译标志

# 生成目标文件
$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# 生成对象文件规则
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@  # 处理 .cpp 文件

# 清理编译生成的文件
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
