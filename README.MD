# Домашнее задание №1 по курсу "Функциональная логика и теория алгоритмов"

### Программа использует стороннюю библиотеку [SFML](https://sfml-dev.org/), перед компиляцией её необходимо установить. Изображения графов генерируются с помощью утилиты [Graphviz](https://graphviz.org/), которую так же необходимо установить перед запуском.

## Компиляция

```bash
g++ main.cpp -o graph -lsfml-window -lsfml-system -lsfml-graphics
```

## Формат входных данных

Программа может считывать матрицу смежности как из стандартного потока ввода-вывода, так и из файла.
Для считывания из файла при запуске указывается имя файла, например:
```bash
./graph input.txt
```

Матрица должна быть представлена квадратной и симметричной относительно главной диагонали, элементы разделяются пробелами, строки - переводом строки.

## Формат выходных данных

На выходе программа выводит окно с изображением графа, сгенерированным с помощью утилиты graphviz. В консоли выводится информация о наличии циклов в графе.

*Выполнил студент группы ИУ4-32Б Бардик Эдуард. Вариант 1.*
