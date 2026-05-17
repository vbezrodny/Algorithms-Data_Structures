from typing import Any, Optional, Iterator


class Node:
    def __init__(self, data: Any):
        self.data = data
        self.next: Optional['Node'] = None


class LinkedQueue:
    def __init__(self):
        self.head: Optional[Node] = None
        self.tail: Optional[Node] = None
        self.size: int = 0

    def enqueue(self, item: Any) -> None:
        new_node = Node(item)
        if self.head is None:
            self.head = self.tail = new_node
        else:
            self.tail.next = new_node
            self.tail = new_node

        self.size += 1

    def dequeue(self) -> Any:
        if self.head is None:
            raise IndexError("dequeue from empty queue")

        data = self.head.data
        self.head = self.head.next

        if self.head is None:
            self.tail = None

        self.size -= 1

        return data

    def front(self) -> Any:
        if self.head is None:
            raise IndexError("front from empty queue")

        return self.head.data

    def __contains__(self, item: Any) -> bool:
        return self.contains(item)

    def contains(self, item: Any) -> bool:
        current = self.head
        while current:
            if current.data == item:
                return True
            current = current.next
        return False

    def index(self, item: Any) -> int:
        idx = 0
        current = self.head
        while current:
            if current.data == item:
                return idx
            idx += 1
            current = current.next
        raise ValueError(f"{item} not in queue")

    def update_at(self, index: int, new_item: Any) -> None:
        if index < 0 or index >= self.size:
            raise IndexError("queue index out of range")

        current = self.head
        for _ in range(index):
            current = current.next

        current.data = new_item

    def remove(self, item: Any) -> None:
        if self.head is None:
            raise ValueError(f"{item} not in queue")

        if self.head.data == item:
            self.dequeue()
            return

        current = self.head
        while current.next:
            if current.next.data == item:
                if current.next == self.tail:
                    self.tail = current
                current.next = current.next.next
                self.size -= 1
                return
            current = current.next
        raise ValueError(f"{item} not in queue")

    def delete_at(self, index: int) -> None:
        if index < 0 or index >= self.size:
            raise IndexError("queue index out of range")

        if index == 0:
            self.dequeue()
            return

        current = self.head
        for _ in range(index - 1):
            current = current.next

        if current.next == self.tail:
            self.tail = current

        current.next = current.next.next
        self.size -= 1

    def __len__(self) -> int:
        return self.size

    def size(self) -> int:
        return self.size

    def is_empty(self) -> bool:
        return self.size == 0

    def clear(self) -> None:
        self.head = self.tail = None
        self.size = 0

    def __iter__(self) -> Iterator[Any]:
        current = self.head
        while current:
            yield current.data
            current = current.next

    def __str__(self) -> str:
        items = list(self)
        return f"LinkedQueue: head -> {items} <- tail"

    def __copy__(self) -> 'LinkedQueue':
        new_q = LinkedQueue()
        for item in self:
            new_q.enqueue(item)
        return new_q
