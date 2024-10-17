#ifndef MYLIST_HPP
#define MYLIST_HPP

#include <stdexcept>  // Для обработки ошибок
#include <iostream>   // Для отладки

template<typename T>
class MyList {
private:
    T* data;           // Указатель на массив элементов
    int capacity;      // Вместимость списка
    int length;        // Текущая длина списка

    // Метод для расширения массива, если список переполнен
    void resize(int newCapacity) {
        T* newData = new T[newCapacity];  // Выделяем новый массив
        for (int i = 0; i < length; i++) {
            newData[i] = data[i];         // Копируем старые данные в новый массив
        }
        delete[] data;                    // Освобождаем старый массив
        data = newData;                   // Переназначаем указатель на новый массив
        capacity = newCapacity;           // Обновляем вместимость
    }

public:
    // Конструктор по умолчанию
    MyList(int initialCapacity = 10) {
        if (initialCapacity <= 0) {
            throw std::invalid_argument("Вместимость списка должна быть больше 0");
        }
        capacity = initialCapacity;
        data = new T[capacity];           // Выделение памяти для массива данных
        length = 0;                       // Изначально список пуст
    }

    // Деструктор
    ~MyList() {
        delete[] data;  // Освобождаем выделенную память
    }

    // Добавление элемента в конец списка
    void add(const T& value) {
        if (length == capacity) {
            resize(capacity * 2);  // Удваиваем размер при заполнении
        }
        data[length] = value;      // Добавляем элемент в конец
        length++;
    }

    // Вставка элемента по индексу
    void insert(int index, const T& value) {
        if (index < 0 || index > length) {
            throw std::out_of_range("Индекс за пределами диапазона");
        }
        if (length == capacity) {
            resize(capacity * 2);  // Удваиваем размер при заполнении
        }
        for (int i = length; i > index; i--) {
            data[i] = data[i - 1];  // Сдвигаем элементы вправо
        }
        data[index] = value;        // Вставляем новый элемент
        length++;
    }

    // Удаление элемента по индексу
    void removeAt(int index) {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Индекс за пределами диапазона");
        }
        for (int i = index; i < length - 1; i++) {
            data[i] = data[i + 1];  // Сдвигаем элементы влево
        }
        length--;
    }

    // Получение элемента по индексу
    T& get(int index) const {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Индекс за пределами диапазона");
        }
        return data[index];  // Возвращаем элемент
    }

    // Установка значения элемента по индексу
    void set(int index, const T& value) {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Индекс за пределами диапазона");
        }
        data[index] = value;  // Устанавливаем значение
    }

    // Получение текущего размера списка
    int size() const {
        return length;
    }

    // Проверка на пустоту
    bool isEmpty() const {
        return length == 0;
    }

    // Очистка списка
    void clear() {
        length = 0;
    }

    // Печать списка для отладки
    void print() const {
        std::cout << "[";
        for (int i = 0; i < length; i++) {
            std::cout << data[i];
            if (i < length - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";
    }
};

#endif
