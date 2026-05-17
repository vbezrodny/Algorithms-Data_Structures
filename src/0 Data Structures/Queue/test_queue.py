import pytest
import copy
from linked_queue import LinkedQueue
from array_queue import ArrayQueue

@pytest.fixture(params=[LinkedQueue, ArrayQueue])
def queue_class(request):
    """Возвращает класс очереди."""
    return request.param

@pytest.fixture
def empty_queue(queue_class):
    """Создаёт пустую очередь для каждого теста."""
    return queue_class()

@pytest.fixture
def filled_queue(queue_class):
    """Создаёт очередь с тремя элементами."""
    q = queue_class()
    for v in [10, 20, 30]:
        q.enqueue(v)
    return q

def queue_content(q):
    """Возвращает строку с содержимым очереди."""
    return str(list(q))


def test_enqueue_dequeue_order(filled_queue):
    q = filled_queue
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    assert q.dequeue() == 10
    print(f"[Результат]: очередь после dequeue(): {queue_content(q)}")
    assert q.dequeue() == 20
    print(f"[Результат]: очередь после dequeue(): {queue_content(q)}")
    assert q.dequeue() == 30
    print(f"[Результат]: очередь после dequeue(): {queue_content(q)}")
    assert q.is_empty()
    print(f"[Результат]: очередь пуста - {queue_content(q)}")


def test_front(filled_queue):
    q = filled_queue
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    first = q.front()
    assert first == 10
    assert len(q) == 3
    print(f"[Результат]: front() = {first}, очередь: {queue_content(q)}")


def test_contains(empty_queue):
    q = empty_queue
    for ch in ['x', 'y', 'z']:
        q.enqueue(ch)
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    assert ('y' in q) == True
    assert ('w' in q) == False
    assert q.contains('z') == True
    print("[Результат]: 'y' in q → True, 'w' in q → False, contains('z') → True")


def test_index(empty_queue):
    q = empty_queue
    for ch in ['a', 'b', 'c']:
        q.enqueue(ch)
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    idx = q.index('b')
    assert idx == 1
    print(f"[Результат]: index('b') = {idx}")


def test_update_at(empty_queue):
    q = empty_queue
    for v in [1, 2, 3]:
        q.enqueue(v)
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    q.update_at(1, 99)
    print(f"[После update_at(1,99)]: {queue_content(q)}")
    assert q.dequeue() == 1
    print(f"[Результат]: очередь после dequeue(): {queue_content(q)}")
    assert q.dequeue() == 99
    print(f"[Результат]: очередь после dequeue(): {queue_content(q)}")
    print("[Результат]: dequeue вернул 1, затем 99")


def test_remove(empty_queue):
    q = empty_queue
    for v in [7, 8, 9]:
        q.enqueue(v)
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    q.remove(8)
    print(f"[После remove(8)]: {queue_content(q)}")
    assert q.dequeue() == 7
    print(f"[Результат]: очередь после dequeue(): {queue_content(q)}")
    assert q.dequeue() == 9
    print(f"[Результат]: очередь после dequeue(): {queue_content(q)}")
    assert q.is_empty()
    print("[Результат]: удалён элемент 8, порядок извлечения 7, 9")


def test_delete_at(empty_queue):
    q = empty_queue
    for v in [100, 200, 300]:
        q.enqueue(v)
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    q.delete_at(1)
    print(f"[После delete_at(1)]: {queue_content(q)}")
    assert q.dequeue() == 100
    print(f"[Результат]: очередь после dequeue(): {queue_content(q)}")
    assert q.dequeue() == 300
    print(f"[Результат]: очередь после dequeue(): {queue_content(q)}")
    assert q.is_empty()
    print("[Результат]: удалён элемент 200, порядок извлечения 100, 300")


def test_clear(empty_queue):
    q = empty_queue
    for v in [1, 2, 3]:
        q.enqueue(v)
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    q.clear()
    print(f"[После clear()]: {queue_content(q)}")
    assert q.is_empty()
    assert len(q) == 0
    print("[Результат]: очередь очищена, is_empty() = True")


def test_iteration(empty_queue):
    q = empty_queue
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    for v in [4, 5, 6]:
        q.enqueue(v)
    items = list(q)
    assert items == [4, 5, 6]
    print(f"[Итерация]: {items} → порядок сохранён")


def test_copy(empty_queue):
    q = empty_queue
    for v in [1, 2]:
        q.enqueue(v)
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    q2 = copy.copy(q)
    print(f"[Копирование]: оригинал {list(q)}, копия {list(q2)}")
    q2.dequeue()
    print(f"[Результат]: копия после dequeue() {list(q2)}")
    assert len(q) == 2
    assert len(q2) == 1


def test_dequeue_empty_raises(empty_queue):
    q = empty_queue
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    with pytest.raises(IndexError):
        q.dequeue()
    print("[Результат]: получен IndexError при dequeue из пустой очереди")


def test_front_empty_raises(empty_queue):
    q = empty_queue
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    with pytest.raises(IndexError):
        q.front()
    print("[Результат]: получен IndexError при front из пустой очереди")


def test_remove_missing_raises(empty_queue):
    q = empty_queue
    q.enqueue(1)
    q.enqueue(2)
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    with pytest.raises(ValueError):
        q.remove(99)
    print("[Результат]: получен ValueError при удалении отсутствующего элемента 99")


def test_update_at_bad_index_raises(empty_queue):
    q = empty_queue
    q.enqueue(10)
    q.enqueue(20)
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    with pytest.raises(IndexError):
        q.update_at(5, 0)
    print("[Результат]: получен IndexError при неверном индексе 5 в update_at()")


def test_delete_at_bad_index_raises(empty_queue):
    q = empty_queue
    q.enqueue(10)
    q.enqueue(20)
    print(f"\n[Исходная очередь]: {queue_content(q)}")
    with pytest.raises(IndexError):
        q.delete_at(-1)
    print("[Результат]: получен IndexError при неверном индексе -1 в delete_at()")