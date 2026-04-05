#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include <chrono>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <cstring>


using namespace std;
using namespace std::chrono;


// Глобальные счетчики для оценки сложности
unsigned long long comparison_count = 0;
unsigned long long operation_count = 0;

// Сброс счетчиков
void reset_counters() {
    comparison_count = 0;
    operation_count = 0;
}

// Функция для сравнения с подсчетом
template <typename T>
bool compare(const T& a, const T& b) {
    comparison_count++;
    return a < b;
}


// Предварительные объявления
template <typename T> bool is_sorted(const T* array, size_t size);
template <typename T> void insertion_sort(T* array, size_t size);
template <typename T> void counting_sort_float(T* array, size_t size);
template <typename T> void modified_quick_sort(T* array, size_t size, size_t M = 10);


// Функция-обертка для modified_quick_sort с фиксированным M
template <size_t M>
void modified_quick_sort_wrapper(float* array, size_t size) {
    modified_quick_sort(array, size, M);
}


// (re-)ordered sequence
template <typename T>
void generate_ordered_seq(T* array, size_t size, T min_value, T max_value, T interval)
{
    if (!size) return;

    if constexpr (std::is_floating_point_v<T>) {
        const T step = (max_value - min_value) / static_cast<T>(size - 1);
        for (size_t i = 0; i < size; ++i) {
            array[i] = min_value + static_cast<T>(i) * step;
        }
    } else {
        for (size_t i = 0; i < size; ++i) {
            array[i] = min_value + static_cast<T>(i);
        }
    }
}

template <typename T>
void generate_reordered_seq(T* array, size_t size, T min_value, T max_value, T interval)
{
    if (!size) return;

    if constexpr (std::is_floating_point_v<T>) {
        const T step = (max_value - min_value) / static_cast<T>(size - 1);
        for (size_t i = 0; i < size; ++i) {
            array[i] = max_value - static_cast<T>(i) * step;
        }
    } else {
        for (size_t i = 0; i < size; ++i) {
            array[i] = max_value - static_cast<T>(i);
        }
    }
}

// random sequence
template <typename T>
void generate_random_seq(T* array, size_t size, T min_value, T max_value, T interval)
{
    if (!size) return;

    random_device rd;
    mt19937 gen(rd());

    if constexpr (is_integral_v<T>)
    {
        uniform_int_distribution<T> dis(min_value, max_value);
        for (size_t i = 0; i < size; ++i)
        {
            array[i] = dis(gen);
        }
    }
    else if constexpr (is_floating_point_v<T>)
    {
        uniform_real_distribution<T> dis(min_value, max_value);
        for (size_t i = 0; i < size; ++i)
        {
            array[i] = dis(gen);
        }
    }
}

// Генерация квазиупорядоченной последовательности
template <typename T>
void generate_quasi_seq(T* array, size_t size, T min_value, T max_value, T interval) {
    if (!size || !interval) return;

    random_device rd;
    mt19937 gen(rd());

    if constexpr (is_integral_v<T>) {
        uniform_int_distribution<T> dis(min_value, interval);
        for (size_t i = 0; i < size; ++i) {
            array[i] = (i % 2 == 1) ? static_cast<T>(i) - dis(gen)
                                   : static_cast<T>(i) + dis(gen);
        }
    }
    else if constexpr (is_floating_point_v<T>) {
        uniform_real_distribution<T> dis(min_value, interval);
        for (size_t i = 0; i < size; ++i) {
            array[i] = (i % 2 == 1) ? static_cast<T>(i) - dis(gen)
                                   : static_cast<T>(i) + dis(gen);
        }
    }
}


// Реализация is_sorted с подсчетом операций
template <typename T>
bool is_sorted(const T* array, size_t size) {
    for (size_t i = 1; i < size; ++i) {
        comparison_count++;  // Считаем все сравнения
        operation_count++;
        if (array[i - 1] > array[i]) {
            return false;
        }
    }
    return true;
}

// Модифицированная сортировка вставками с подсчетом операций
template <typename T>
void insertion_sort(T* array, size_t size) {
    for (size_t i = 1; i < size; ++i) {
        operation_count += 2;
        T key = array[i];
        int j = static_cast<int>(i) - 1;

        while (j >= 0) {
            comparison_count++;
            operation_count++;
            if (!compare(array[j], key)) {  // Инвертированное условие
                operation_count += 2;
                array[j + 1] = array[j];
                j--;
            } else {
                break;
            }
        }
        operation_count++;
        array[j + 1] = key;
    }
}

// Модифицированная сортировка подсчетом с подсчетом операций
template <typename T>
void counting_sort_float(T* array, size_t size) {
    if (size == 0) return;

    operation_count += 2; // инициализация min_val и max_val
    T min_val = array[0];
    T max_val = array[0];

    for (size_t i = 1; i < size; ++i) {
        operation_count += 2; // сравнения в if
        if (compare(array[i], min_val)) {
            operation_count++;
            min_val = array[i];
        }
        if (array[i] > max_val) { // не используем compare, так как это не операция сравнения элементов
            comparison_count++;
            operation_count++;
            max_val = array[i];
        }
    }

    const size_t range = static_cast<size_t>(max_val - min_val) + 1;
    operation_count++; // сравнение в if
    if (range > 1000000) {
        insertion_sort(array, size);
        return;
    }

    operation_count++; // создание вектора
    vector<size_t> count(range, 0);

    for (size_t i = 0; i < size; ++i) {
        operation_count++; // инкремент в count
        count[static_cast<size_t>(array[i] - min_val)]++;
    }

    size_t index = 0;
    operation_count++; // инициализация index
    for (size_t i = 0; i < range; ++i) {
        operation_count++; // сравнение в while
        while (count[i] > 0) {
            operation_count += 2; // присваивание и декремент
            array[index++] = static_cast<T>(i) + min_val;
            count[i]--;
        }
    }
}

// Модифицированная быстрая сортировка с подсчетом операций
template <typename T>
void modified_quick_sort(T* array, size_t size, size_t M) {
    operation_count++;
    if (size <= M) {
        insertion_sort(array, size);
        return;
    }

    // Выбор медианы из трех
    size_t mid = size / 2;
    operation_count += 3;
    if (compare(array[size-1], array[0])) swap(array[0], array[size-1]);
    if (compare(array[mid], array[0])) swap(array[0], array[mid]);
    if (compare(array[size-1], array[mid])) swap(array[mid], array[size-1]);

    T pivot = array[mid];
    operation_count++;

    // Разделение
    size_t i = 0, j = size - 1;
    operation_count += 2;

    while (i <= j) {
        operation_count++;
        while (compare(array[i], pivot)) {
            operation_count++;
            i++;
        }
        operation_count++;
        while (compare(pivot, array[j])) {
            operation_count++;
            j--;
        }
        operation_count++;
        if (i <= j) {
            operation_count += 3;
            swap(array[i], array[j]);
            i++;
            j--;
        }
    }

    operation_count++;
    if (j > 0) modified_quick_sort(array, j + 1, M);
    operation_count++;
    if (i < size) modified_quick_sort(array + i, size - i, M);
}

// Функция для тестирования с оценкой сложности
template <typename T, typename Func>
void test_sort_with_complexity(Func sort_func, const string& name, T* array, size_t size) {
    cout << "Testing " << name << " with n = " << size << "... ";

    reset_counters();
    auto start = high_resolution_clock::now();
    sort_func(array, size);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    if (is_sorted(array, size)) {
        cout << "Sorted OK. Time: " << duration.count() << " μs\n";
        cout << "Comparisons: " << comparison_count << "\n";
        cout << "Total operations: " << operation_count << "\n";

        // Оценка временной сложности
        double time_complexity = duration.count() / (size * log2(size));
        cout << "Time complexity coefficient: " << time_complexity << "\n";
    } else {
        cout << "Sorting FAILED!\n";
    }
    cout << endl;
}

// Функция для тестирования на разных размерах массивов
template <typename T>
void complexity_analysis(void (*sort_func)(T*, size_t), const string& name) {
    const size_t sizes[] = {100, 1000, 10000, 50000};
    const int num_tests = sizeof(sizes)/sizeof(sizes[0]);

    cout << "===== Complexity Analysis for " << name << " =====\n";

    for (int i = 0; i < num_tests; ++i) {
        size_t size = sizes[i];
        T* array = new T[size];

        generate_quasi_seq(array, size, -1000.0f, 1000.0f, 500.0f);
        test_sort_with_complexity(sort_func, name, array, size);

        delete[] array;
    }
}


// Компаратор для qsort
int compare_qsort(const void* a, const void* b) {
    comparison_count++;
    float fa = *((float*)a);
    float fb = *((float*)b);
    return (fa > fb) - (fa < fb);
}

// Тестирование алгоритма
template <typename T, typename Func>
void test_algorithm(Func sort_func, const string& algo_name,
                   const string& seq_type, T* array, size_t size) {
    reset_counters();

    auto start = high_resolution_clock::now();
    sort_func(array, size);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    // Запись результатов в файл
    ofstream out("results.csv", ios::app);
    out << algo_name << "," << seq_type << "," << size << ","
        << duration.count() << "," << comparison_count << ","
        << operation_count << "\n";
    out.close();
}

// Основная функция анализа
void run_complexity_analysis() {
    const size_t sizes[] = {5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000};
    const int num_sizes = sizeof(sizes)/sizeof(sizes[0]);

    // Создаем заголовок CSV
    ofstream out("results.csv");
    out << "Algorithm,SequenceType,Size,Time(us),Comparisons,Operations\n";
    out.close();

    for (int i = 0; i < num_sizes; ++i) {
        size_t size = sizes[i];
        float* array = new float[size];

        // Тестируем на разных типах последовательностей
        const char* seq_types[] = {"ordered", "reordered", "random", "quasi"};

        for (const char* seq_type : seq_types) {
            // Генерируем последовательность
            if (strcmp(seq_type, "ordered") == 0) {
                generate_ordered_seq(array, size, -1000.0f, 1000.0f, 0.0f);
            }
            else if (strcmp(seq_type, "reordered") == 0) {
                generate_reordered_seq(array, size, -1000.0f, 1000.0f, 0.0f);
            }
            else if (strcmp(seq_type, "random") == 0) {
                generate_random_seq(array, size, -1000.0f, 1000.0f, 0.0f);
            }
            else if (strcmp(seq_type, "quasi") == 0) {
                generate_quasi_seq(array, size, -1000.0f, 1000.0f, 500.0f);
            }

            // Создаем копию для каждого алгоритма
            float* array_copy = new float[size];

            // Тестируем разные алгоритмы
            vector<pair<string, void(*)(float*, size_t)>> algorithms = {
                {"InsertionSort", insertion_sort<float>},
                {"QuickSort", [](float* a, size_t s) { modified_quick_sort(a, s, 15); }},
                {"CountingSort", counting_sort_float<float>}
            };

            for (auto& algo : algorithms) {
                copy(array, array + size, array_copy);
                test_algorithm(algo.second, algo.first, seq_type, array_copy, size);
            }

            // Тестируем qsort
            copy(array, array + size, array_copy);
            reset_counters();
            auto start = high_resolution_clock::now();
            qsort(array_copy, size, sizeof(float), compare_qsort);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);

            ofstream out("results.csv", ios::app);
            out << "Qsort," << seq_type << "," << size << ","
                << duration.count() << "," << comparison_count << ",0\n";
            out.close();

            delete[] array_copy;
        }

        delete[] array;
    }
}


int main() {
    run_complexity_analysis();
    cout << "Analysis complete. Results saved to results.csv" << endl;
    return 0;
}
