import csv
import datetime as dt
import random as rnd
import math as m


def measure_sequence_duration(seq_func):
    def wrapper(*args, **kwargs):
        t_start = dt.datetime.now()
        seq_func(*args, **kwargs)
        t_finish = dt.datetime.now()

        return str(t_finish - t_start)

    return wrapper


@measure_sequence_duration
def create_ordered_sequence(arr, arr_size, range_min, range_max, interval_length, is_seq_float):
    if is_seq_float:
        fr_step = (range_max - range_min) / (arr_size - 1)
        for i in range(0, arr_size + 1):
            arr.append(range_min + i * fr_step)
    else:
        for i in range(0, arr_size + 1):
            arr.append(range_min + i)


@measure_sequence_duration
def create_reordered_sequence(arr, arr_size, range_min, range_max, interval_length, is_seq_float):
    if is_seq_float:
        fr_step = (range_max - range_min) / (arr_size - 1)
        for i in range(0, arr_size + 1):
            arr.append(range_max - i * fr_step)
    else:
        for i in range(0, arr_size + 1):
            arr.append(range_max - i)


@measure_sequence_duration
def create_random_sequence(arr, arr_size, range_min, range_max, interval_length, is_seq_float):
    if is_seq_float:
        for i in range(0, arr_size + 1):
            arr.append(rnd.uniform(range_min, range_max))
    else:
        for i in range(0, arr_size + 1):
            arr.append(rnd.randint(range_min, range_max))


@measure_sequence_duration
def create_saw_sequence(arr, arr_size, range_min, range_max, interval_length, is_seq_float):
    slope = (range_max - range_min) / interval_length

    if is_seq_float:
        fr_step = (range_max - range_min) / (arr_size - 1)
        for k in range(0, arr_size + 1, interval_length):
            for i in range(0, interval_length + 1):
                arr.append(range_min + i * slope + fr_step)
    else:
        for k in range(0, arr_size + 1, interval_length):
            for i in range(0, interval_length + 1):
                arr.append(range_min + i * slope)


@measure_sequence_duration
def create_sinusoidal_sequence(arr, arr_size, range_min, range_max, interval_length, is_seq_float):
    amplitude = (range_max - range_min) / 2
    center = (range_max + range_min) / 2

    for i in range(0, arr_size + 1):
        arr.append(amplitude * m.sin(2 * m.pi * i / interval_length) + center)


@measure_sequence_duration
def create_step_sequence(arr, arr_size, range_min, range_max, interval_length, is_seq_float):
    min_rnd = -interval_length / 10
    max_rnd = interval_length / 10

    if is_seq_float:
        for k in range(0, arr_size + 1, interval_length):
            for i in range(0, interval_length + 1):
                arr.append(k + rnd.uniform(min_rnd, max_rnd))
    else:
        for k in range(0, arr_size + 1, interval_length):
            for i in range(0, interval_length + 1):
                arr.append(k + rnd.randint(min_rnd, max_rnd))


@measure_sequence_duration
def create_quasi_ordered_sequence(arr, arr_size, range_min, range_max, interval_length, is_seq_float):
    min_rnd = -interval_length / 2
    max_rnd = interval_length / 2

    if is_seq_float:
        for i in range(0, arr_size + 1):
            if i % 2 == 0:
                arr.append(i + rnd.uniform(min_rnd, max_rnd))
            else:
                arr.append(i - rnd.uniform(min_rnd, max_rnd))
    else:
        for i in range(0, arr_size + 1):
            if i % 2 == 0:
                arr.append(i + rnd.randint(min_rnd, max_rnd))
            else:
                arr.append(i - rnd.randint(min_rnd, max_rnd))


def log_sequences(arr, arr_size, arr_max_size, range_min, range_max, interval_length, is_seq_float, file_name):
    step = arr_size

    sequences = [
        create_ordered_sequence,
        create_reordered_sequence,
        create_random_sequence,
        create_saw_sequence,
        create_sinusoidal_sequence,
        create_step_sequence,
        create_quasi_ordered_sequence
    ]

    if is_seq_float:
        type_numbers = 'float numbers'
    else:
        type_numbers = 'int numbers'

    # csv-file: Логирование последовательностей с заданным параметрами
    # with open("Последовательности.csv", 'w') as csvfile:
    #     writer = csv.writer(csvfile, delimiter=';')
    #
    #     writer.writerow([type_numbers, 'ordered', 'reodered', 'random', 'saw', 'sinusoidal', 'step', 'quasi ordered'])
    #
    #     results_in_line = []
    #
    #     for seq in sequences:
    #         arr.clear()
    #
    #         seq(arr, 10000, 0, 1000, 100,
    #             is_float_numbers)
    #         results_in_line.append(arr)
    #
    #     writer.writerow([1000, *results_in_line])
    #     results_in_line.clear()
    #     arr.clear()

    # csv-file: Логирование длительности формирования последовательностей
    with open(file_name, 'w') as csvfile:
        writer = csv.writer(csvfile, delimiter=';')

        writer.writerow([type_numbers, 'ordered', 'reodered', 'random', 'saw', 'sinusoidal', 'step', 'quasi ordered'])

        results_in_line = []

        while arr_size <= arr_max_size:
            for seq in sequences:
                t_result = seq(arr, arr_size, range_min, range_max, interval_length, is_seq_float)
                results_in_line.append(t_result)

                arr.clear()

            writer.writerow([arr_size, *results_in_line])
            results_in_line.clear()

            arr_size += step


if __name__ == '__main__':
    sequence_array = []
    size = 5 * 10 ** 5
    max_size = 50 * 10 ** 5

    range_min = 0
    range_max = 10_000
    interval_length = 1000

    is_float_numbers = False
    log_sequences(sequence_array, size, max_size, range_min, range_max, interval_length, is_float_numbers,
                  'int.csv')

    is_float_numbers = True
    log_sequences(sequence_array, size, max_size, range_min, range_max, interval_length, is_float_numbers,
                  'float.csv')
