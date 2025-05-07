# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17
TARGET = tram_system

# Файлы для компиляции
SRC = main.cpp tram_manager.cpp
OBJ = $(SRC:.cpp=.o)

# Псевдоцели
.PHONY: all clean run

# Основная цель сборки
all: $(TARGET)

# Сборка исполняемого файла из объектных
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Правило для компиляции .cpp в .o
%.o: %.cpp tram_manager.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка артефактов
clean:
	rm -f $(OBJ) $(TARGET)

# Запуск программы
run: $(TARGET)
	./$(TARGET)
