#ifndef MYHASHMAP_H
#define MYHASHMAP_H

#include <iostream>
#include <string>
#include <stdexcept>  // Для обработки ошибок
#include "MyList.hpp"

// Структура для хранения значения
template <typename TK, typename TV>
struct NodeMap {
    TK key;  // Ключ
    TV value;  // Значение
    NodeMap* next;  // Указатель на следующий узел (для обработки коллизий)

    // Конструктор для NodeMap
    NodeMap(const TK& k, const TV& v) : key(k), value(v), next(nullptr) {}
};

// Структура для хранения ключа и значения
template <typename TK, typename TV>
class MyHashMap {
private:
    NodeMap<TK, TV>** data;  // Массив указателей на узлы
    int length;  // Количество элементов в хэш-таблице
    int capacity;  // Вместимость хэш-таблицы
    int LoadFactor;  // Фактор загрузки (процент заполнения, при котором происходит расширение)

public:
    // Конструктор
    MyHashMap(int initCapacity, int initLoadFactor) {
        if (initCapacity <= 0 || initLoadFactor <= 0 || initLoadFactor > 100) {
            throw std::runtime_error("Индекс вне диапазона");  // Выбрасываем ошибку, если параметры некорректны
        }
        capacity = initCapacity;
        LoadFactor = initLoadFactor;
        length = 0;
        data = new NodeMap<TK, TV>*[capacity]();  // Выделяем память под массив указателей и инициализируем нулями
    }

    // Деструктор
    ~MyHashMap() {
        DestroyMap();  // Освобождаем память
    }

    // Оператор разыменования
    TV& operator*(const TK& key) {
        // Найти индекс по хэш-функции
        int index = HashCode(key) % capacity;
        NodeMap<TK, TV>* curr = data[index];

        // Проход по цепочке для поиска ключа
        while (curr != nullptr) {
            if (curr->key == key) {
                return curr->value;  // Возвращаем значение, если ключ найден
            }
            curr = curr->next;  // Переходим к следующему узлу
        }

        throw std::runtime_error("Ключ не найден");  // Выбрасываем исключение, если ключ не найден
    }

    // Метод для расширения хэш-таблицы
    void Expansion() {
        int newCap = capacity * 2;  // Новая вместимость в два раза больше текущей
        NodeMap<TK, TV>** newData = new NodeMap<TK, TV>*[newCap]();  // Новый массив указателей
        for (int i = 0; i < capacity; i++) {
            NodeMap<TK, TV>* curr = data[i];
            while (curr != nullptr) {
                NodeMap<TK, TV>* next = curr->next;
                int index = HashCode(curr->key) % newCap;  // Вычисляем новый индекс
                curr->next = newData[index];  // Перемещаем узел в новый массив
                newData[index] = curr;
                curr = next;
            }
        }
        delete[] data;  // Освобождаем старый массив
        data = newData;  // Устанавливаем новый массив
        capacity = newCap;  // Обновляем вместимость
    }

    // Добавление элементов
    void AddMap(const TK& key, const TV& value) {
        if ((length + 1) * 100 / capacity >= LoadFactor) {
            Expansion();  // Если достигнут фактор загрузки, расширяем хэш-таблицу
        }
        int index = HashCode(key) % capacity;  // Вычисляем индекс
        NodeMap<TK, TV>* newNode = new NodeMap<TK, TV>(key, value);  // Создаем новый узел
        newNode->next = data[index];  // Добавляем узел в начало цепочки
        data[index] = newNode;  // Обновляем массив
        length++;  // Увеличиваем количество элементов
    }

    // Поиск элементов по ключу
    TV GetMap(const TK& key) const {
        int index = HashCode(key) % capacity;  // Вычисляем индекс
        NodeMap<TK, TV>* curr = data[index];
        while (curr != nullptr) {
            if (curr->key == key) {
                return curr->value;  // Возвращаем значение, если ключ найден
            }
            curr = curr->next;
        }
        throw std::runtime_error("Ключ не найден");  // Выбрасываем ошибку, если ключ не найден
    }

    // Удаление элементов
    void DeleteMap(const TK& key) {
        int index = HashCode(key) % capacity;  // Вычисляем индекс
        NodeMap<TK, TV>* curr = data[index];
        NodeMap<TK, TV>* prev = nullptr;
        while (curr != nullptr) {
            if (curr->key == key) {
                if (prev == nullptr) {
                    data[index] = curr->next;  // Удаляем первый элемент в цепочке
                } else {
                    prev->next = curr->next;  // Удаляем элемент из середины или конца цепочки
                }
                delete curr;  // Освобождаем память
                length--;  // Уменьшаем количество элементов
                return;
            }
            prev = curr;
            curr = curr->next;
        }
        throw std::runtime_error("Ключ не найден");  // Выбрасываем ошибку, если ключ не найден
    }

    // Очистка памяти
    void DestroyMap() {
        for (int i = 0; i < capacity; i++) {
            NodeMap<TK, TV>* curr = data[i];
            while (curr != nullptr) {
                NodeMap<TK, TV>* next = curr->next;
                delete curr;  // Освобождаем память каждого узла
                curr = next;
            }
        }
        delete[] data;  // Освобождаем память массива указателей
        data = nullptr;
        length = 0;
        capacity = 0;
    }

    // Хэш-функция для ключа типа int
    static int HashCode(const TK& key) {
        unsigned long hash = 5381;  // Начальное значение хэша
        for (char ch : key) {
            hash = ((hash << 5) + hash) + ch;  // hash * 33 + c
        }
        return hash;
    }



};

#endif  // MYHASHMAP_H
