#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iomanip>
#include <functional>

using namespace std;

// Класс для измерения количества сравнений
class ComparisonCounter {
public:
    size_t count = 0;

    void reset() { count = 0; }
    void inc() { ++count; }
    size_t get() const { return count; }
};

// 1. Последовательный поиск (оптимизированная версия)
template <typename T>
size_t sequential_search(const T* array, size_t size, T key, ComparisonCounter& counter) {
    for (size_t i = 0; i < size; ++i) {
        counter.inc(); // Учитываем только сравнение элементов
        if (array[i] >= key) { // Объединенное сравнение
            counter.inc(); // Дополнительная проверка на равенство
            return (array[i] == key) ? i : size;
        }
    }
    return size;
}

// 2. Бинарный поиск (оптимизированная версия)
template <typename T>
size_t binary_search(const T* array, size_t size, T key, ComparisonCounter& counter) {
    size_t left = 0;
    size_t right = size - 1;

    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        counter.inc(); // Основное сравнение

        if (array[mid] < key) {
            left = mid + 1;
        }
        else {
            counter.inc(); // Учитываем только ветку else
            if (array[mid] == key) return mid;
            right = mid - 1;
        }
    }
    return size;
}

// 3. Интерполяционный поиск (исправленная версия)
template <typename T>
size_t interpolation_search(const T* array, size_t size, T key, ComparisonCounter& counter) {
    size_t low = 0;
    size_t high = size - 1;

    while (low <= high) {
        counter.inc(); // Проверка границ диапазона

        if (key < array[low] || key > array[high]) {
            return size;
        }

        if (low == high) {
            counter.inc();
            return (array[low] == key) ? low : size;
        }

        size_t pos = low + ((double)(high - low) / (array[high] - array[low])) * (key - array[low]);
        pos = min(pos, high - 1); // Защита от выхода за границы

        counter.inc();
        if (array[pos] == key) {
            return pos;
        }

        counter.inc();
        if (array[pos] < key) {
            low = pos + 1;
        } else {
            high = pos - 1;
        }
    }
    return size;
}

// Генерация упорядоченного массива
template <typename T>
vector<T> generate_ordered_array(size_t size, T min_val, T max_val) {
    vector<T> arr(size);
    if constexpr (is_floating_point_v<T>) {
        T step = (max_val - min_val) / (size - 1);
        for (size_t i = 0; i < size; ++i) {
            arr[i] = min_val + i * step;
        }
    } else {
        for (size_t i = 0; i < size; ++i) {
            arr[i] = min_val + i;
        }
    }
    return arr;
}

// Тестирование алгоритма поиска
template <typename T, typename Func>
void test_search_algorithm(
    Func search_func,
    const vector<T>& arr,
    const vector<T>& test_keys,
    size_t& total_comparisons,
    size_t& total_successful
) {
    ComparisonCounter counter;
    total_comparisons = 0;
    total_successful = 0;

    for (const T& key : test_keys) {
        counter.reset();
        size_t result = search_func(arr.data(), arr.size(), key, counter);
        total_comparisons += counter.get();
        if (result != arr.size()) {
            total_successful++;
        }
    }
}

int main() {
    // Параметры исследования
    const vector<size_t> sizes = {100, 1000, 5000, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
    const int test_runs = 1000;
    const int min_val = 0;

    // Заголовок таблицы
    cout << left << setw(15) << "Array size"
         << setw(20) << "Algorithm"
         << setw(25) << "Successful (avg comp)"
         << setw(25) << "Unsuccessful (avg comp)"
         << endl;

    for (size_t size : sizes) {
        const int max_val = size - 1;
        vector<int> arr = generate_ordered_array(size, min_val, max_val);

        // Генерация тестовых ключей
        vector<int> successful_keys;
        vector<int> unsuccessful_keys;

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> successful_dist(0, size-1);
        uniform_int_distribution<> unsuccessful_dist(size, 2*size);

        for (int i = 0; i < test_runs; ++i) {
            successful_keys.push_back(arr[successful_dist(gen)]);
            unsuccessful_keys.push_back(unsuccessful_dist(gen));
        }

        // Тестируем каждый алгоритм
        struct Algorithm {
            string name;
            std::function<size_t(const int*, size_t, int, ComparisonCounter&)> func;
        };

        vector<Algorithm> algorithms = {
            {"Sequential", sequential_search<int>},
            {"Binary", binary_search<int>},
            {"Interpolation", interpolation_search<int>}
        };

        for (const auto& algo : algorithms) {
            size_t successful_comps, successful_count;
            test_search_algorithm(algo.func, arr, successful_keys, successful_comps, successful_count);
            double avg_successful = static_cast<double>(successful_comps) / test_runs;

            size_t unsuccessful_comps, unsuccessful_count;
            test_search_algorithm(algo.func, arr, unsuccessful_keys, unsuccessful_comps, unsuccessful_count);
            double avg_unsuccessful = static_cast<double>(unsuccessful_comps) / test_runs;

            cout << setw(15) << size
                 << setw(20) << algo.name
                 << setw(25) << avg_successful
                 << setw(25) << avg_unsuccessful
                 << endl;
        }

        cout << endl;
    }
    
    return 0;
}