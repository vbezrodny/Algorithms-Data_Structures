from typing import Any, Iterator, Optional
import copy


class ArrayQueue:
    def __init__(self, initial_capacity: int = 4):
        self.data: list = [None] * initial_capacity
        self.head: int = 0
        self.tail: int = 0
        self.size: int = 0
        self.capacity: int = initial_capacity

    def resize(self, new_capacity: int) -> None:
        new_data = [None] * new_capacity
        for i in range(self.size):
            new_data[i] = self.data[(self.head + i) % self.capacity]

        self.data = new_data
        self.head = 0
        self.tail = self.size
        self.capacity = new_capacity

    def enqueue(self, item: Any) -> None:
        if self.size == self.capacity:
            self.resize(2 * self.capacity)

        self.data[self.tail] = item
        self.tail = (self.tail + 1) % self.capacity
        self.size += 1

    def dequeue(self) -> Any:
        if self.size == 0:
            raise IndexError("dequeue from empty queue")

        item = self.data[self.head]
        self.data[self.head] = None
        self.head = (self.head + 1) % self.capacity
        self.size -= 1

        return item

    def front(self) -> Any:
        if self.size == 0:
            raise IndexError("front from empty queue")

        return self.data[self.head]

    def __contains__(self, item: Any) -> bool:
        return self.contains(item)

    def contains(self, item: Any) -> bool:
        for i in range(self.size):
            if self.data[(self.head + i) % self.capacity] == item:
                return True
        return False

    def index(self, item: Any) -> int:
        for i in range(self.size):
            if self.data[(self.head + i) % self.capacity] == item:
                return i
        raise ValueError(f"{item} not in queue")

    def update_at(self, index: int, new_item: Any) -> None:
        if index < 0 or index >= self.size:
            raise IndexError("queue index out of range")

        real_index = (self.head + index) % self.capacity
        self.data[real_index] = new_item

    def remove(self, item: Any) -> None:
        idx = self.index(item)
        self.delete_at(idx)

    def delete_at(self, index: int) -> None:
        if index < 0 or index >= self.size:
            raise IndexError("queue index out of range")

        if index == 0:
            self.dequeue()
            return

        # сдвиг элементов
        for i in range(index, self.size - 1):
            curr_real = (self.head + i) % self.capacity
            next_real = (self.head + i + 1) % self.capacity
            self.data[curr_real] = self.data[next_real]

        # очистка последней позиции
        last_real = (self.head + self.size - 1) % self.capacity
        self.data[last_real] = None
        self.tail = (self.tail - 1) % self.capacity
        self.size -= 1

    def __len__(self) -> int:
        return self.size

    def size(self) -> int:
        return self.size

    def is_empty(self) -> bool:
        return self.size == 0

    def clear(self) -> None:
        self.data = [None] * self.capacity
        self.head = 0
        self.tail = 0
        self.size = 0

    def __iter__(self) -> Iterator[Any]:
        for i in range(self.size):
            yield self.data[(self.head + i) % self.capacity]

    def __str__(self) -> str:
        items = list(self)
        return f"ArrayQueue: [{', '.join(map(str, items))}]"

    def __copy__(self) -> 'ArrayQueue':
        new_q = ArrayQueue(self.capacity)
        new_q.data = self.data.copy()
        new_q.head = self.head
        new_q.tail = self.tail
        new_q.size = self.size
        return new_q
