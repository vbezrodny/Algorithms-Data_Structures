import time
import csv
from linked_queue import LinkedQueue
from array_queue import ArrayQueue


def measure_time(func, repeats=5):
    def wrapper(*args, **kwargs):
        total = 0.0
        for _ in range(repeats):
            start = time.perf_counter()
            func(*args, **kwargs)
            total += time.perf_counter() - start

        return total / repeats

    return wrapper


def run_research():
    sizes = [5_000, 10_000, 50_000, 500_000, 1_000_000]
    results = []

    for n in sizes:
        print(f"Измерение для n={n:_}")

        lq = LinkedQueue()
        aq = ArrayQueue(initial_capacity=n)
        for i in range(1, n + 1):
            lq.enqueue(i)
            aq.enqueue(i)

        def search_lq():
            return -1 in lq

        def search_aq():
            return -1 in aq

        if n <= 50_000:
            repeats = 10
        elif n <= 500_000:
            repeats = 5
        else:
            repeats = 3

        t_lq = measure_time(search_lq, repeats)()
        t_aq = measure_time(search_aq, repeats)()

        results.append((n, f"{t_lq:.6f}", f"{t_aq:.6f}"))
        print(f"  LinkedQueue: {t_lq:.6f} сек, ArrayQueue: {t_aq:.6f} сек")

    with open('search_times.csv', 'w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(['n', 'LinkedQueue', 'ArrayQueue'])
        writer.writerows(results)

    print("\nРезультаты сохранены в search_times.csv")
    print("Таблица результатов:")
    print("n\t\tLinkedQueue (сек)\tArrayQueue (сек)")
    for n, t_lq, t_aq in results:
        print(f"{n}\t\t{float(t_lq):.6f}\t\t{float(t_aq):.6f}")


if __name__ == "__main__":
    run_research()
