# Network Security Analysis System

Система анализа сетевой безопасности, реализующая классификацию сетевых атак с помощью алгоритма K-Nearest Neighbors и шифрование пакетов с помощью алгоритма Blowfish.

## Структура проекта

## Требования

- C++17 компилятор (g++ 7+, clang 5+, MSVC 2017+)
- CMake 3.10+
- Python 3.6+ (для подготовки данных)

## Сборка

```bash
# Клонирование репозитория (если нужно)
git clone <repository-url>
cd network-security-analysis

# Создание директории сборки
mkdir build
cd build

# Конфигурация проекта
cmake ..

# Сборка
cmake --build . --config Release

# Альтернативно
make -j4