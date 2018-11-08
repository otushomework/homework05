#include <iostream>
#include <vector>
#include <map>
#include <assert.h>

//#define ENABLE_2D_MATRIX
//#define ENABLE_3D_MATRIX

using Index = std::vector<int>;

template<typename T, T DefVal = -1, size_t Dimension = 2>
class Matrix
{
public:
    Matrix() = default;

    ~Matrix() { }

    auto size()  { return m_data.size();  }
    auto begin() { return m_data.begin(); }
    auto end()   { return m_data.end();   }

    template<class ContainerType>
    struct Proxy
    {
        ContainerType *container;
        Index index;
        Proxy(ContainerType *a, Index index) : container(a), index(index) {}

        operator T()
        {
            assert(index.size() == Dimension);

            if (container->m_data.find(index) != container->m_data.end())
                return container->m_data[index];
            else
                return DefVal;
        }

        T& operator=(const T& other)
        {
            assert(index.size() == Dimension);

            //do not store default values
            if (other == DefVal)
                return const_cast<T&>(other);

            return container->m_data[index] = other;
        }
    };

    auto operator[](Index index)
    {
        return Proxy<Matrix<T, DefVal, Dimension> >(this, index);
    }

private:
    std::map<Index, T> m_data;
};

int main(int, char *[])
{
#ifdef ENABLE_2D_MATRIX
    Matrix<int, -1, 2> matrix2d;
    assert(matrix2d.size() == 0); // все ячейки свободны

    auto a2d = matrix2d[Index({0,0})];
    assert(a2d == -1);
    assert(matrix2d.size() == 0);

    matrix2d[Index({100,100})] = 314;
    assert(matrix2d[Index({100,100})] == 314);
    assert(matrix2d.size() == 1);

    // выведется одна строка
    // 100100314
    for(auto c : matrix2d)
    {
        for(auto v : c.first)
            std::cout << v;
        std::cout << c.second << std::endl;
    }
#endif

#ifdef ENABLE_3D_MATRIX
    Matrix<int, -1, 3> matrix3d;
    assert(matrix3d.size() == 0); // все ячейки свободны

    auto a3d = matrix3d[Index({0,0,0})];
    assert(a3d == -1);
    assert(matrix3d.size() == 0);

    matrix3d[Index({100, 100, 100})] = 314;
    assert(matrix3d[Index({100, 100, 100})] == 314);
    assert(matrix3d.size() == 1);

    // выведется одна строка
    // 100100100314
    for(auto c : matrix3d)
    {
        for(auto v : c.first)
            std::cout << v;
        std::cout << c.second << std::endl;
    }
#endif

    //fill matrix
    Matrix<int, 0, 2> matrix;
    for(int i = 0, j = 0; i <= 9; ++i, ++j)
    {
        matrix[Index({i, i})] = i;
        matrix[Index({9 - i, i})] = 9 - i;
    }

    //print matrix fragment
    for(int i = 1; i <= 8; ++i)
    {
        for(int j = 1; j <= 8; ++j)
        {
            std::cout << matrix[Index({i, j})] << (j < 8 ? " " : "");
        }
        std::cout << std::endl;
    }

    assert(matrix.size() == 18);
    std::cout << matrix.size() << std::endl;

    for(auto c : matrix)
    {
        for(auto v : c.first)
            std::cout << v;
        std::cout << c.second << std::endl;
    }

    ((matrix[Index({100, 100})] = 314) = 0) = 217;
    //std::cout << matrix[Index({100, 100})] << std::endl;

    return 0;
}
