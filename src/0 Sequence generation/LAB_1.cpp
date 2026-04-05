#include <iostream>
#include <cstddef>
#include <vector>
#include <random>
#include <cmath>
#include <chrono>
#include <fstream>


using namespace std;
using namespace std::chrono;

template <typename T>
using SequenceGenerator = void (*)
(
	T* array,
	size_t size,
	T min_value,
	T max_value,
	T interval
);


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

// saw sequence
template <typename T>
void generate_saw_seq(T* array, size_t size, T min_value, T max_value, T interval)
{
	if (size == 0 || interval <= 0) return;

	if constexpr (std::is_floating_point_v<T>) {
		const T period = interval;
		const T amplitude = max_value - min_value;

		for (size_t i = 0; i < size; ++i) {
			T pos_in_period = std::fmod(static_cast<T>(i), period) / period;
			array[i] = min_value + amplitude * pos_in_period;
		}
	} else {
		for (size_t i = 0; i < size; i += static_cast<size_t>(interval)) {
			for (size_t j = 0; j < static_cast<size_t>(interval) && (i + j) < size; ++j) {
				array[i + j] = min_value + static_cast<T>(j);
			}
		}
	}
}

// sinus sequence
template <typename T>
void generate_sin_seq(T* array, size_t size, T min_value, T max_value, T interval)
{
	if (!size || !interval) return;

	const T amplitude = (max_value - min_value) / 2;
	const T offset = (min_value + max_value) / 2;

	for (size_t i = 0; i < size; ++i)
	{
		array[i] = amplitude * sin(2 * M_PI * static_cast<T>(i) / interval) + offset;
	}
}

// step sequence
template <typename T>
void generate_step_seq(T* array, size_t size, T min_value, T max_value, T interval)
{
	if (!size || !interval) return;

	random_device rd;
	mt19937 gen(rd());

	if constexpr (is_integral_v<T>)
	{
		uniform_int_distribution<T> dis(min_value, interval);
		for (size_t i = 0; i < size; i += interval)
		{
			for (size_t j = 0; j < interval && (i + j) < size; ++j)
			{
				array[i + j] = dis(gen) + i;
			}
		}
	}
	else if constexpr (is_floating_point_v<T>)
	{
		uniform_real_distribution<T> dis(min_value, interval);
		for (size_t i = 0; i < size; i += interval)
		{
			for (size_t j = 0; j < interval && (i + j) < size; ++j)
			{
				array[i + j] = dis(gen) + i;
			}
		}
	}
}

// quasi sequence
template <typename T>
void generate_quasi_seq(T* array, size_t size, T min_value, T max_value, T interval)
{
	if (!size || !interval) return;

	random_device rd;
	mt19937 gen(rd());

	if constexpr (is_integral_v<T>)
	{
		uniform_int_distribution<T> dis(min_value, interval);

		for (size_t i = 0; i < size; ++i)
		{
			if (i % 2 == 1) array[i] = static_cast<T>(i) - dis(gen);
			else array[i] = static_cast<T>(i) + dis(gen);
		}
	}
	else if constexpr (is_floating_point_v<T>)
	{
		uniform_real_distribution<T> dis(min_value, interval);

		for (size_t i = 0; i < size; ++i)
		{
			if (i % 2 == 1) array[i] = static_cast<T>(i) - dis(gen);
			else array[i] = static_cast<T>(i) + dis(gen);
		}
	}
}


template <typename T>
using SequenceGenerator = void (*)(T*, size_t, T, T, T);

template <typename T>
void measure_generators(const vector<size_t>& sizes, const string& filename) {
	ofstream outfile(filename);
	outfile << "Size,Ordered,Reordered,Random,Saw,Sinus,Step,Quasi\n";

	const T min_val = 0;
	const T max_val = 100;
	const T interval = 10;

	SequenceGenerator<T> generators[] = {
		generate_ordered_seq<T>,
		generate_reordered_seq<T>,
		generate_random_seq<T>,
		generate_saw_seq<T>,
		generate_sin_seq<T>,
		generate_step_seq<T>,
		generate_quasi_seq<T>
	};

	const char* generator_names[] = {
		"Ordered", "Reordered", "Random", "Saw", "Sinus", "Step", "Quasi"
	};

	for (size_t size : sizes) {
		vector<T> arr(size);
		outfile << size;

		for (auto generator : generators) {
			auto start = high_resolution_clock::now();

			generator(arr.data(), size, min_val, max_val, interval);

			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);

			outfile << "," << duration.count();
		}
		outfile << "\n";
		cout << "Completed size: " << size << endl;
	}

	outfile.close();
}


int main() {
	vector<size_t> sizes;
	for (size_t i = 5; i <= 50; i += 5) {
		sizes.push_back(i * 100000); // 500000, 1000000, ..., 5000000
	}

	measure_generators<int>(sizes, "timings_int.csv");

	measure_generators<double>(sizes, "timings_double.csv");

	cout << "Measurements completed. Data saved to timings_int.csv and timings_double.csv" << endl;

	return 0;
}
