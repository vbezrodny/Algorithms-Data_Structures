#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <cfloat>
#include <ctime>

#include <windows.h>


struct Deikstra {
    bool   Label;
    double Path;
    int    Vertex;
    Deikstra() : Label(false), Path(DBL_MAX), Vertex(-1) {}
};


class Matrix {
protected:
    double** _m;
    int _Size;
public:
    Matrix(int Size);
    Matrix(const Matrix& M);
    double operator() (int row, int col);
    int Size(void) { return _Size; }
    void Set(int row, int col, double Value);
    ~Matrix();
};


Matrix::Matrix(int Size) : _Size(0), _m(0)
{
    if (Size > 0) {
        _Size = Size;
        _m = new double* [_Size];
        for (int i = 0; i < _Size; i++)
            _m[i] = new double[_Size];
    }
}


Matrix::Matrix(const Matrix& M)
{
    if (&M != this) {
        _Size = 0;
        _m = 0;
        if (M._Size > 0) {
            _Size = M._Size;
            _m = new double* [_Size];
            for (int i = 0; i < _Size; i++)
                _m[i] = new double[_Size];
        }
        for (int i = 0; i < _Size; i++)
            for (int j = 0; j < _Size; j++)
                _m[i][j] = M._m[i][j];
    }
}


double Matrix::operator() (int row, int col)
{
    if (row < _Size && col < _Size && row >= 0 && col >= 0)
        return _m[row][col];
    return DBL_MAX;
}


void Matrix::Set(int row, int col, double Value)
{
    if (row < _Size && col < _Size && row >= 0 && col >= 0)
        _m[row][col] = Value;
}


Matrix::~Matrix()
{
    for (int i = 0; i < _Size; i++)
        delete[] _m[i];
    delete[] _m;
}


class Graph : public Matrix
{
public:
    Graph(int Size) : Matrix(Size) {
        for (int i = 0; i < _Size; i++)
            for (int j = 0; j < _Size; j++)
                if (i != j) _m[i][j] = DBL_MAX;
    }
    Graph(const Graph& G) : Matrix(G) {}

    virtual void AddEdge(int V0, int V1, double Weight = 1);
    void DeleteEdge(int V0, int V1);
    void Print(void);
    int EdgeCount(void);
    int Degree(int v);
    bool IsConnected(void);

    void FindCitiesWithinDistance(int from, double maxDistance);

    void GenerateEulerianGraph(int edgeDensity);
    void GenerateHamiltonianGraph(int edgeDensity);

    bool HasEulerianCycle();
    void FindEulerianCycle();
    void FindHamiltonianCycle();

protected:
    void DFSUtil(int v, bool* visited);
    void EulerianUtil(int v, bool** usedEdges, int* path, int& pathIndex);
    bool HamiltonianUtil(int v, int* path, bool* visited, int depth);
};


void Graph::AddEdge(int V0, int V1, double Weight)
{
    if (V0 >= 0 && V1 >= 0 && V0 < _Size && V1 < _Size && V0 != V1 && Weight >= 0) {
        _m[V0][V1] = Weight;
    }
}


void Graph::DeleteEdge(int V0, int V1)
{
    if (V0 >= 0 && V1 >= 0 && V0 < _Size && V1 < _Size && V0 != V1) {
        _m[V0][V1] = DBL_MAX;
    }
}


void Graph::Print(void)
{
    for (int i = 0; i < _Size; i++) {
        for (int j = 0; j < _Size; j++)
            if (_m[i][j] < DBL_MAX)
                printf("%7.1lf", _m[i][j]);
            else
                printf("     oo");
        printf("\n");
    }
    printf("\n");
}


int Graph::EdgeCount(void)
{
    if (_Size == 0) return 0;
    int Count = 0;
    for (int i = 0; i < _Size; i++)
        for (int j = 0; j < _Size; j++)
            if (i != j && _m[i][j] < DBL_MAX)
                Count++;
    return Count;
}


int Graph::Degree(int v)
{
    if (v < 0 || v >= _Size) return 0;
    int deg = 0;
    for (int i = 0; i < _Size; i++)
        if (i != v && _m[v][i] < DBL_MAX)
            deg++;
    return deg;
}


void Graph::DFSUtil(int v, bool* visited)
{
    visited[v] = true;
    for (int u = 0; u < _Size; u++) {
        if (u != v && _m[v][u] < DBL_MAX && !visited[u]) {
            DFSUtil(u, visited);
        }
    }
}


bool Graph::IsConnected(void)
{
    if (_Size == 0) return false;

    bool* visited = new bool[_Size];
    for (int i = 0; i < _Size; i++) visited[i] = false;

    // Находим первую вершину с ненулевой степенью
    int start = -1;
    for (int i = 0; i < _Size; i++) {
        if (Degree(i) > 0) {
            start = i;
            break;
        }
    }

    if (start == -1) {
        delete[] visited;
        return false;
    }

    DFSUtil(start, visited);

    // Проверяем, все ли вершины с ненулевой степенью посещены
    for (int i = 0; i < _Size; i++) {
        if (Degree(i) > 0 && !visited[i]) {
            delete[] visited;
            return false;
        }
    }

    delete[] visited;
    return true;
}


void Graph::FindCitiesWithinDistance(int from, double maxDistance)
{
    if (from < 0 || from >= _Size) {
        printf("Неверная вершина-источник.\n");
        return;
    }

    Deikstra* D = new Deikstra[_Size];
    D[from].Path = 0;
    D[from].Vertex = -1;

    for (int count = 0; count < _Size; count++) {
        double minPath = DBL_MAX;
        int v = -1;
        for (int i = 0; i < _Size; i++) {
            if (!D[i].Label && D[i].Path < minPath) {
                minPath = D[i].Path;
                v = i;
            }
        }
        if (v == -1) break;
        D[v].Label = true;

        for (int u = 0; u < _Size; u++) {
            if (!D[u].Label && _m[v][u] < DBL_MAX) {
                double newDist = D[v].Path + _m[v][u];
                if (newDist < D[u].Path) {
                    D[u].Path = newDist;
                    D[u].Vertex = v;
                }
            }
        }
    }

    printf("Города, расстояние от города %d до которых не превышает %.2lf:\n", from, maxDistance);
    bool found = false;
    for (int i = 0; i < _Size; i++) {
        if (i != from && D[i].Path <= maxDistance && D[i].Path < DBL_MAX) {
            printf("  Город %d (расстояние = %.2lf)\n", i, D[i].Path);
            found = true;
        }
    }
    if (!found) {
        printf("  Таких городов нет.\n");
    }

    delete[] D;
}


void Graph::GenerateEulerianGraph(int edgeDensity)
{
    if (_Size < 2) return;

    for (int i = 0; i < _Size; i++)
        for (int j = 0; j < _Size; j++)
            if (i != j) _m[i][j] = DBL_MAX;

    int maxEdges = _Size * (_Size - 1) / 2;
    int targetEdges = (edgeDensity > maxEdges) ? maxEdges : edgeDensity;
    targetEdges = (targetEdges < _Size) ? _Size : targetEdges;

    int* degree = new int[_Size];
    for (int i = 0; i < _Size; i++) degree[i] = 0;

    for (int i = 0; i < _Size; i++) {
        int next = (i + 1) % _Size;
        if (_m[i][next] >= DBL_MAX) {
            AddEdge(i, next, 1.0);
            degree[i]++;
            degree[next]++;
        }
    }

    int currentEdges = _Size;

    // Добавляем дополнительные рёбра, сохраняя чётность степеней
    while (currentEdges < targetEdges) {
        int v1 = rand() % _Size;
        int v2 = rand() % _Size;

        if (v1 != v2 && _m[v1][v2] >= DBL_MAX) {
            AddEdge(v1, v2, 1.0);
            degree[v1]++;
            degree[v2]++;
            currentEdges++;

            // Если после добавления степень стала нечётной, ищем способ исправить
            if (degree[v1] % 2 != 0 && degree[v2] % 2 != 0) {
                continue;
            } else if (degree[v1] % 2 != 0 || degree[v2] % 2 != 0) {
                DeleteEdge(v1, v2);
                degree[v1]--;
                degree[v2]--;
                currentEdges--;
            }
        }
    }

    // Финальная проверка: если есть вершины с нечётной степенью, добавляем корректирующие рёбра
    for (int i = 0; i < _Size; i++) {
        if (degree[i] % 2 != 0) {
            for (int j = i + 1; j < _Size; j++) {
                if (degree[j] % 2 != 0 && _m[i][j] >= DBL_MAX) {
                    AddEdge(i, j, 1.0);
                    degree[i]++;
                    degree[j]++;
                    break;
                }
            }
        }
    }

    delete[] degree;
    printf("Эйлеров граф сгенерирован (все степени чётные). Рёбер: %d\n", EdgeCount());
}


void Graph::GenerateHamiltonianGraph(int edgeDensity)
{
    if (_Size < 3) return;

    for (int i = 0; i < _Size; i++)
        for (int j = 0; j < _Size; j++)
            if (i != j) _m[i][j] = DBL_MAX;

    int maxEdges = _Size * (_Size - 1) / 2;
    int targetEdges = (edgeDensity > maxEdges) ? maxEdges : edgeDensity;
    targetEdges = (targetEdges < _Size) ? _Size : targetEdges;

    // Создаём гамильтонов цикл (связываем все вершины в кольцо)
    for (int i = 0; i < _Size; i++) {
        int next = (i + 1) % _Size;
        AddEdge(i, next, 1.0);
    }

    int currentEdges = _Size;  // в цикле V рёбер

    // Добавляем случайные дополнительные рёбра (хорды)
    while (currentEdges < targetEdges) {
        int v1 = rand() % _Size;
        int v2 = rand() % _Size;

        if (v1 != v2 && _m[v1][v2] >= DBL_MAX) {
            AddEdge(v1, v2, 1.0);
            currentEdges++;
        }
    }

    printf("Гамильтонов граф сгенерирован (содержит цикл через все %d вершин). Рёбер: %d\n",
           _Size, EdgeCount());
}


bool Graph::HasEulerianCycle() {
    if (!IsConnected()) {
        printf("Граф несвязный\n");
        return false;
    }

    for (int i = 0; i < _Size; i++) {
        if (Degree(i) % 2 != 0) {
            printf("Вершина %d имеет нечётную степень %d\n", i, Degree(i));
            return false;
        }
    }
    return true;
}


void Graph::EulerianUtil(int v, bool** usedEdges, int* path, int& pathIndex)
{
    for (int u = 0; u < _Size; u++) {
        if (u != v && _m[v][u] < DBL_MAX && !usedEdges[v][u]) {
            usedEdges[v][u] = true;
            usedEdges[u][v] = true;
            EulerianUtil(u, usedEdges, path, pathIndex);
        }
    }
    path[pathIndex++] = v;
}


void Graph::FindEulerianCycle()
{
    printf("\n=== ПОИСК ЭЙЛЕРОВА ЦИКЛА ===\n");

    if (!HasEulerianCycle()) {
        printf("Эйлеров цикл не существует!\n");
        return;
    }

    if (!IsConnected()) {
        printf("Граф несвязный, эйлеров цикл невозможен.\n");
        return;
    }

    bool** usedEdges = new bool*[_Size];
    for (int i = 0; i < _Size; i++) {
        usedEdges[i] = new bool[_Size];
        for (int j = 0; j < _Size; j++)
            usedEdges[i][j] = false;
    }

    int* path = new int[_Size * _Size];
    int pathIndex = 0;

    // Начинаем с любой вершины (возьмём вершину 0, если она есть)
    int start = 0;
    for (int i = 0; i < _Size; i++) {
        if (Degree(i) > 0) {
            start = i;
            break;
        }
    }

    EulerianUtil(start, usedEdges, path, pathIndex);

    printf("Эйлеров цикл: ");
    for (int i = pathIndex - 1; i >= 0; i--) {
        printf("%d", path[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");

    for (int i = 0; i < _Size; i++)
        delete[] usedEdges[i];
    delete[] usedEdges;
    delete[] path;
}


bool Graph::HamiltonianUtil(int v, int* path, bool* visited, int depth) {
    path[depth] = v;

    if (depth == _Size - 1) {
        // Проверяем, есть ли ребро от последней вершины к первой
        if (_m[v][path[0]] < DBL_MAX) {
            return true;
        }
        return false;
    }

    visited[v] = true;

    // Перебираем смежные вершины
    for (int u = 0; u < _Size; u++) {
        if (u != v && _m[v][u] < DBL_MAX && !visited[u]) {
            if (HamiltonianUtil(u, path, visited, depth + 1)) {
                return true;
            }
        }
    }

    visited[v] = false;
    return false;
}


void Graph::FindHamiltonianCycle()
{
    printf("\n=== ПОИСК ГАМИЛЬТОНОВА ЦИКЛА ===\n");

    if (_Size < 3) {
        printf("Для гамильтонова цикла нужно минимум 3 вершины.\n");
        return;
    }

    int* path = new int[_Size];
    bool* visited = new bool[_Size];
    for (int i = 0; i < _Size; i++) visited[i] = false;

    if (HamiltonianUtil(0, path, visited, 0)) {
        printf("Гамильтонов цикл найден: ");
        for (int i = 0; i < _Size; i++) {
            printf("%d", path[i]);
            if (i < _Size - 1) printf(" -> ");
        }
        printf(" -> %d\n", path[0]);
    } else {
        printf("Гамильтонов цикл не найден.\n");
    }

    delete[] path;
    delete[] visited;
}


class WGraph : public Graph
{
public:
    WGraph(int Size) : Graph(Size) {}
    WGraph(const WGraph& G) : Graph(G) {}

    virtual void AddEdge(int V0, int V1, double Weight = 1);
};


void WGraph::AddEdge(int V0, int V1, double Weight)
{
    Graph::AddEdge(V0, V1, Weight);
    Graph::AddEdge(V1, V0, Weight);
}


int main(int argc, char* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    srand((unsigned)time(NULL));

    int size = 6;
    WGraph G(size);

    printf("1. Генерация ЭЙЛЕРОВА графа (все степени чётные)\n");
    printf("----------------------------------------\n");
    G.GenerateEulerianGraph(9);
    printf("\nМатрица смежности:\n");
    G.Print();

    printf("Проверка степеней вершин:\n");
    for (int i = 0; i < G.Size(); i++) {
        printf("  Степень вершины %d: %d\n", i, G.Degree(i));
    }
    G.FindEulerianCycle();

    printf("\n\n");

    printf("2. Генерация ГАМИЛЬТОНОВА графа (содержит цикл через все вершины)\n");
    printf("----------------------------------------\n");
    WGraph H(6);
    H.GenerateHamiltonianGraph(10);
    printf("\nМатрица смежности:\n");
    H.Print();

    H.FindHamiltonianCycle();

    getch();
    return 0;
}
