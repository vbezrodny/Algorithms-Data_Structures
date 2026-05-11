#include <iostream>
#include <queue>
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;

// Узел скошенного дерева
template<typename T>
struct SkewNode {
    T key;
    shared_ptr<SkewNode<T>> left;
    shared_ptr<SkewNode<T>> right;

    SkewNode(const T& val) : key(val), left(nullptr), right(nullptr) {}
};

// Скошенное дерево (Skew Heap)
template<typename T>
class SkewHeap {
private:
    shared_ptr<SkewNode<T>> root;

    // Вспомогательная функция слияния двух скошенных деревьев
    shared_ptr<SkewNode<T>> merge(shared_ptr<SkewNode<T>> h1, shared_ptr<SkewNode<T>> h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        // Убеждаемся, что корень h1 содержит меньший ключ (min-heap)
        if (h1->key > h2->key) {
            swap(h1, h2);
        }

        // Рекурсивно сливаем правое поддерево h1 с h2
        // Результат становится новым правым поддеревом
        h1->right = merge(h1->right, h2);

        // Скошивание - меняем местами левое и правое поддеревья
        swap(h1->left, h1->right);

        return h1;
    }

    // Рекурсивный вывод дерева (обход в глубину)
    void printTree(shared_ptr<SkewNode<T>> node, int level, const string& prefix) const {
        if (!node) return;

        // Вывод текущего узла с отступами
        cout << string(level * 4, ' ') << prefix << node->key << endl;

        // Рекурсивный вывод детей
        if (node->left || node->right) {
            if (node->left) {
                printTree(node->left, level + 1, "L: ");
            } else {
                cout << string((level + 1) * 4, ' ') << "L: null" << endl;
            }

            if (node->right) {
                printTree(node->right, level + 1, "R: ");
            } else {
                cout << string((level + 1) * 4, ' ') << "R: null" << endl;
            }
        }
    }

    // Копирование дерева
    shared_ptr<SkewNode<T>> copyTree(shared_ptr<SkewNode<T>> node) {
        if (!node) return nullptr;
        auto newNode = make_shared<SkewNode<T>>(node->key);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }

    // Очистка дерева
    void clearTree(shared_ptr<SkewNode<T>>& node) {
        if (!node) return;
        clearTree(node->left);
        clearTree(node->right);
        node.reset();
    }

    // Проверка на наличие ключа
    bool contains(shared_ptr<SkewNode<T>> node, const T& key) const {
        if (!node) return false;
        if (node->key == key) return true;

        // В скошенной куче нет структуры BST, поэтому нужно искать везде
        return contains(node->left, key) || contains(node->right, key);
    }

    // Получение размера дерева
    int getSize(shared_ptr<SkewNode<T>> node) const {
        if (!node) return 0;
        return 1 + getSize(node->left) + getSize(node->right);
    }

public:
    SkewHeap() : root(nullptr) {}

    // Конструктор копирования
    SkewHeap(const SkewHeap& other) {
        root = copyTree(other.root);
    }

    // Оператор присваивания
    SkewHeap& operator=(const SkewHeap& other) {
        if (this != &other) {
            clear();
            root = copyTree(other.root);
        }
        return *this;
    }

    // Деструктор
    ~SkewHeap() {
        clear();
    }

    // Вставка ключа (основная операция)
    void insert(const T& key) {
        auto newNode = make_shared<SkewNode<T>>(key);
        root = merge(root, newNode);
    }

    // Удаление минимального ключа
    void extractMin() {
        if (!root) {
            throw runtime_error("Heap is empty");
        }
        root = merge(root->left, root->right);
    }

    // Получение минимального ключа
    T getMin() const {
        if (!root) {
            throw runtime_error("Heap is empty");
        }
        return root->key;
    }

    // Слияние двух куч
    void mergeHeap(SkewHeap& other) {
        root = merge(root, other.root);
        other.root = nullptr;
    }

    // Проверка на пустоту
    bool isEmpty() const {
        return root == nullptr;
    }

    // Очистка кучи
    void clear() {
        clearTree(root);
    }

    // Поиск ключа
    bool contains(const T& key) const {
        return contains(root, key);
    }

    // Получение размера
    int size() const {
        return getSize(root);
    }

    // Вывод дерева на экран
    void display() const {
        if (!root) {
            cout << "Tree is empty" << endl;
            return;
        }
        cout << "\n=== Skew Heap Structure ===" << endl;
        printTree(root, 0, "Root: ");
        cout << "==========================" << endl;
    }

    // Обход в ширину (уровневый вывод)
    void levelOrderTraversal() const {
        if (!root) {
            cout << "Tree is empty" << endl;
            return;
        }

        cout << "\nLevel-order traversal: ";
        queue<shared_ptr<SkewNode<T>>> q;
        q.push(root);

        while (!q.empty()) {
            auto node = q.front();
            q.pop();

            cout << node->key << " ";

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        cout << endl;
    }

    // Вывод всех ключей в порядке возрастания (через извлечение минимума)
    void printSorted() const {
        if (!root) {
            cout << "Tree is empty" << endl;
            return;
        }

        // Создаем копию для извлечения элементов
        SkewHeap<T> temp = *this;

        cout << "\nSorted elements: ";
        while (!temp.isEmpty()) {
            cout << temp.getMin() << " ";
            temp.extractMin();
        }
        cout << endl;
    }
};

// Тестовое приложение
void printMenu() {
    cout << "\n========== Skew Heap Menu ==========" << endl;
    cout << "1. Insert key" << endl;
    cout << "2. Get minimum key" << endl;
    cout << "3. Extract minimum key" << endl;
    cout << "4. Check if key exists" << endl;
    cout << "5. Get heap size" << endl;
    cout << "6. Display heap structure" << endl;
    cout << "7. Level-order traversal" << endl;
    cout << "8. Print sorted elements" << endl;
    cout << "9. Merge with another heap" << endl;
    cout << "10. Clear heap" << endl;
    cout << "0. Exit" << endl;
    cout << "====================================" << endl;
    cout << "Choice: ";
}

void testMerge() {
    cout << "\n--- Merge Test ---" << endl;
    SkewHeap<int> heap1, heap2;

    // Создаем первую кучу
    vector<int> values1 = {10, 5, 15, 3, 8};
    for (int val : values1) {
        heap1.insert(val);
    }

    // Создаем вторую кучу
    vector<int> values2 = {20, 1, 25, 7, 12};
    for (int val : values2) {
        heap2.insert(val);
    }

    cout << "Heap 1 before merge: ";
    heap1.printSorted();
    cout << "Heap 2 before merge: ";
    heap2.printSorted();

    heap1.mergeHeap(heap2);

    cout << "Heap 1 after merge: ";
    heap1.printSorted();
    cout << "Heap 2 is " << (heap2.isEmpty() ? "empty" : "not empty") << endl;
}

int main() {
    SkewHeap<int> heap;
    int choice, key;

    cout << "=== Skew Heap Implementation in C++ ===" << endl;
    cout << "Demo: Insertion operation in Skew Heap" << endl;

    // Демонстрация базовых операций
    cout << "\n--- Initial Demo ---" << endl;
    vector<int> demoKeys = {15, 10, 20, 5, 25, 8, 3};
    cout << "Inserting keys: ";
    for (int k : demoKeys) {
        cout << k << " ";
        heap.insert(k);
    }
    cout << endl;

    heap.display();
    cout << "Minimum key: " << heap.getMin() << endl;
    cout << "Heap size: " << heap.size() << endl;

    // Интерактивное тестирование
    do {
        printMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter key to insert: ";
                cin >> key;
                heap.insert(key);
                cout << "Key " << key << " inserted successfully." << endl;
                break;

            case 2:
                try {
                    cout << "Minimum key: " << heap.getMin() << endl;
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;

            case 3:
                try {
                    int minKey = heap.getMin();
                    heap.extractMin();
                    cout << "Extracted minimum key: " << minKey << endl;
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;

            case 4:
                cout << "Enter key to search: ";
                cin >> key;
                if (heap.contains(key)) {
                    cout << "Key " << key << " found in heap." << endl;
                } else {
                    cout << "Key " << key << " not found." << endl;
                }
                break;

            case 5:
                cout << "Heap size: " << heap.size() << endl;
                break;

            case 6:
                heap.display();
                break;

            case 7:
                heap.levelOrderTraversal();
                break;

            case 8:
                heap.printSorted();
                break;

            case 9:
                testMerge();
                break;

            case 10:
                heap.clear();
                cout << "Heap cleared." << endl;
                break;

            case 0:
                cout << "Exiting program..." << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 0);

    return 0;
}