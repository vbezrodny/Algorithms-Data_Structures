#include <iostream>
#include <cstring>
#include <chrono>
#include <iomanip>
#include <random>
#include <vector>

#define BITAP_LIMIT 32
#define TEXT_SIZE 100000
#define PATTERN_SIZE 10
#define ITERATIONS 1000

using namespace std;
using namespace std::chrono;

int SearchStringSO(const unsigned char* T, int N, const unsigned char* P, int M);
int SearchStringSTR(const unsigned char* T, int N, const unsigned char* P, int M);
unsigned char* generateRandomText(int size, int seed = 42);
unsigned char* generateRandomPattern(int size, int seed = 123);
template<typename Func> double measureAvgTime(Func func, int iterations);
void testTextSize();
void testPatternSize();
void testTextSizeUnsuccessful();
void testPatternSizeUnsuccessful();

int main()
{
    testTextSize();
    testPatternSize();
    testTextSizeUnsuccessful();
    testPatternSizeUnsuccessful();

    return 0;
}

int SearchStringSO(const unsigned char* T, int N, const unsigned char* P, int M)
{
    if (M == 0) return 0;
    if (M > BITAP_LIMIT) return -1;

    unsigned int SC[256], R = 0xFFFFFFFF, Mask = 0;

    int i;
    for (i = 0; i < 256; i++)
        SC[i] = 0xFFFFFFFF;
    for (i = 0; i < M; i++)
        SC[static_cast<unsigned int>(P[i])] &= ~(1 << i);
    Mask = ~(1 << (M - 1));
    for (i = 0; i < N; i++) {
        R = (R << 1) | SC[static_cast<unsigned int>(T[i])];
        if (Mask >= R) return (i - M + 1);
    }

    return -1;
}

int SearchStringSTR(const unsigned char* T, int N, const unsigned char* P, int M)
{
    const char* result = strstr(reinterpret_cast<const char*>(T), reinterpret_cast<const char*>(P));
    if (result == nullptr) return -1;
    return result - reinterpret_cast<const char*>(T);
}

unsigned char* generateRandomText(int size, int seed)
{
    auto* text = new unsigned char[size + 1];

    mt19937 rng(seed);
    uniform_int_distribution<int> dist('a', 'z');

    for (int i = 0; i < size; i++) {
        text[i] = dist(rng);
    }
    text[size] = '\0';

    return text;
}

unsigned char* generateRandomPattern(int size, int seed)
{
    auto* pattern = new unsigned char[size + 1];

    mt19937 rng(seed);
    uniform_int_distribution<int> dist('a', 'z');

    for (int i = 0; i < size; i++) {
        pattern[i] = dist(rng);
    }
    pattern[size] = '\0';

    return pattern;
}

template<typename Func>
double measureAvgTime(Func func, int iterations)
{
    long long duration = 0;
    for (int i = 0; i < iterations; i++) {
        auto start = high_resolution_clock::now();
        func();
        auto stop = high_resolution_clock::now();
        duration += duration_cast<microseconds>(stop - start).count();
    }
    return static_cast<double>(duration) / static_cast<double>(iterations);
}

void testTextSize()
{
    cout << "TEST 1: Search time depends on text size (successful search)" << endl;
    cout << setw(15) << "Text size"
         << setw(20) << "Bitap (mcs)"
         << setw(20) << "strstr (mcs)"
         << setw(20) << "difference" << endl;

    vector<int> sizes = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};

    for (int size : sizes) {
        unsigned char* text = generateRandomText(size);
        unsigned char* pattern = generateRandomPattern(size);

        // Text contains pattern
        int insert_pos = size / 2;
        for (int i = 0; i < PATTERN_SIZE && insert_pos + i < size; i++) {
            text[insert_pos + i] = pattern[i];
        }

        // Bitap time measure
        double bitap_avg = measureAvgTime([&]() {
                                                SearchStringSO(text, size, pattern, PATTERN_SIZE);
                                            },
                                            ITERATIONS);
        // strstr time measure
        double strstr_avg = measureAvgTime([&]() {
                                                SearchStringSTR(text, size, pattern, PATTERN_SIZE);
                                            },
                                            ITERATIONS);

        cout << setw(15) << size
             << setw(20) << fixed << setprecision(3) << bitap_avg
             << setw(20) << strstr_avg
             << setw(15) << (bitap_avg / strstr_avg) << endl;

        delete[] text;
        delete[] pattern;
    }
}

void testPatternSize()
{
    cout << "TEST 2: Search time depends on pattern size (successful search)" << endl;
    cout << setw(15) << "Text size"
         << setw(20) << "Bitap (mcs)"
         << setw(20) << "strstr (mcs)"
         << setw(20) << "difference" << endl;

    vector<int> sizes = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 25, 30};

    for (int pattern_size : sizes) {
        if (pattern_size > BITAP_LIMIT) continue;

        unsigned char* text = generateRandomText(TEXT_SIZE);
        unsigned char* pattern = generateRandomPattern(pattern_size);

        // Text contains pattern
        int insert_pos = TEXT_SIZE / 2;
        for (int i = 0; i < pattern_size && insert_pos + i < TEXT_SIZE; i++) {
            text[insert_pos + i] = pattern[i];
        }

        // Bitap time measure
        double bitap_avg = measureAvgTime([&]() {
                                                SearchStringSO(text, TEXT_SIZE, pattern, pattern_size);
                                            },
                                            ITERATIONS);
        // strstr time measure
        double strstr_avg = measureAvgTime([&] {
                                                SearchStringSTR(text, TEXT_SIZE, pattern, pattern_size);
                                            },
                                            ITERATIONS);

        cout << setw(15) << pattern_size
             << setw(20) << fixed << setprecision(3) << bitap_avg
             << setw(20) << strstr_avg
             << setw(15) << (bitap_avg / strstr_avg) << endl;

        delete[] text;
        delete[] pattern;
    }
}

void testTextSizeUnsuccessful()
{
    cout << "TEST 3: Search time depends on text size (unsuccessful search)" << endl;
    cout << setw(15) << "Text size"
         << setw(20) << "Bitap (mcs)"
         << setw(20) << "strstr (mcs)"
         << setw(20) << "difference" << endl;

    vector<int> sizes = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};

    for (int size : sizes) {
        unsigned char* text = generateRandomText(size);
        unsigned char* pattern = generateRandomPattern(size);

        // Text doesnt contains pattern
        for (int i = 0; i < PATTERN_SIZE; i++) {
            text[i] = '@';
        }

        // Bitap time measure
        double bitap_avg = measureAvgTime([&]() {
                                                SearchStringSO(text, size, pattern, PATTERN_SIZE);
                                            },
                                            ITERATIONS);
        // strstr time measure
        double strstr_avg = measureAvgTime([&]() {
                                                SearchStringSTR(text, size, pattern, PATTERN_SIZE);
                                            },
                                            ITERATIONS);

        cout << setw(15) << size
             << setw(20) << fixed << setprecision(3) << bitap_avg
             << setw(20) << strstr_avg
             << setw(15) << (bitap_avg / strstr_avg) << endl;

        delete[] text;
        delete[] pattern;
    }
}

void testPatternSizeUnsuccessful()
{
    cout << "TEST 4: Search time depends on pattern size (unsuccessful search)" << endl;
    cout << setw(15) << "Text size"
         << setw(20) << "Bitap (mcs)"
         << setw(20) << "strstr (mcs)"
         << setw(20) << "difference" << endl;

    vector<int> sizes = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 25, 30};

    for (int pattern_size : sizes) {
        if (pattern_size > BITAP_LIMIT) continue;

        unsigned char* text = generateRandomText(TEXT_SIZE);
        unsigned char* pattern = generateRandomPattern(pattern_size);

        // Text doesnt contains pattern
        for (int i = 0; i < pattern_size; i++) {
            text[i] = '@';
        }

        // Bitap time measure
        double bitap_avg = measureAvgTime([&]() {
                                                SearchStringSO(text, TEXT_SIZE, pattern, pattern_size);
                                            },
                                            ITERATIONS);
        // strstr time measure
        double strstr_avg = measureAvgTime([&] {
                                                SearchStringSTR(text, TEXT_SIZE, pattern, pattern_size);
                                            },
                                            ITERATIONS);

        cout << setw(15) << pattern_size
             << setw(20) << fixed << setprecision(3) << bitap_avg
             << setw(20) << strstr_avg
             << setw(15) << (bitap_avg / strstr_avg) << endl;

        delete[] text;
        delete[] pattern;
    }
}