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


class Graph : public Matrix {
public:
    Graph(int Size) : Matrix(Size) {
        for (int i = 0; i < _Size; i++)
            for (int j = 0; j < _Size; j++)
                if (i != j) _m[i][j] = DBL_MAX;
    }
    Graph(const Graph& G) : Matrix(G) {}

    virtual void AddEdge(int V0, int V1, double Weight = 1);
    void Print(void);

    void FindCitiesWithinDistance(int from, double maxDistance);
};


void Graph::AddEdge(int V0, int V1, double Weight)
{
    if (V0 >= 0 && V1 >= 0 && V0 < _Size && V1 < _Size && V0 != V1 && Weight >= 0) {
        _m[V0][V1] = Weight;
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


class WGraph : public Graph {
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

    WGraph G(5);

    G.AddEdge(0, 1, 10.0);
    G.AddEdge(0, 2, 5.0);
    G.AddEdge(1, 2, 2.0);
    G.AddEdge(1, 3, 8.0);
    G.AddEdge(2, 4, 7.0);
    G.AddEdge(3, 4, 12.0);

    printf("Матрица смежности графа (oo - дороги нет):\n");
    G.Print();

    int A = 0;
    double L = 12.0;

    G.FindCitiesWithinDistance(A, L);

    getch();
    return 0;
}
