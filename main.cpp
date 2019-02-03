#include <iostream>
#include <vector>
#include <map>
#include <assert.h>

//#define ENABLE_2D_MATRIX
//#define ENABLE_3D_MATRIX

template<typename T, T DefVal = -1, size_t Dimension = 2>
class Matrix
{
public:
    Matrix() = default;

    ~Matrix() { }

    auto size()  { return m_data.size();  }
    auto begin() { return m_data.begin(); }
    auto end()   { return m_data.end();   }

    //рекурсивный Proxy класс
    template<class ContainerType, size_t SubDimension>
    struct Proxy
    {
    private:
        ContainerType *m_container;
        std::vector<int> m_index;
    public:

        Proxy(ContainerType *a, std::vector<int> index) : m_container(a), m_index(index)
        { }

        auto operator[](int i)
        {
            m_index.push_back(i);
            return Proxy<ContainerType, SubDimension - 1>(m_container, m_index);
        }
    };

    //конец рекурсии Proxy-класса
    template<class ContainerType>
    struct Proxy<ContainerType, 0>
    {
    private:
        ContainerType *m_container;
        std::vector<int> m_index;
    public:

        Proxy(ContainerType *a, std::vector<int> index) : m_container(a), m_index(index)
        { }

        auto& operator = (const T& other)
        {

            //do not store default values
            if (other == DefVal)
            {
                m_container->m_data.erase(m_index);
                return const_cast<T&>(other);
            }

            return m_container->m_data[m_index] = other;
        }

        operator T()
        {
            if (m_container->m_data.find(m_index) != m_container->m_data.end())
                return m_container->m_data[m_index];
            else
                return DefVal;
        }
    };

    auto operator[](int i)
    {
        std::vector<int> index;
        index.push_back(i);
        return Proxy<Matrix<T, DefVal, Dimension>, Dimension - 1 >(this, index);
    }

private:
    std::map<std::vector<int>, T> m_data;
};

int main(int, char *[])
{
#ifdef ENABLE_2D_MATRIX
    Matrix<int, -1, 2> matrix2d;
    assert(matrix2d.size() == 0); // все ячейки свободны

    auto a2d = matrix2d[2][4];
    assert(a2d == -1);
    assert(matrix2d.size() == 0);

    matrix2d[100][100] = 314;
    assert(matrix2d[100][100] == 314);
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

    auto a3d = matrix3d[0][0][0];
    assert(a3d == -1);
    assert(matrix3d.size() == 0);

    matrix3d[100][100][100] = 314;
    assert(matrix3d[100][100][100] == 314);
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
        matrix[i][i] = i;
        matrix[9 - i][i] = 9 - i;
    }

    //print matrix fragment
    for(int i = 1; i <= 8; ++i)
    {
        for(int j = 1; j <= 8; ++j)
        {
            std::cout << matrix[i][j] << (j < 8 ? " " : "");
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

        ((matrix[100][100] = 314) = 0) = 217;
        assert(matrix[100][100] == 217);
        assert(matrix.size() == 19);
        //std::cout << matrix(100, 100) << std::endl; //print 217

        //check removing of element
        matrix[100][100] = 314;
        assert(matrix.size() == 19);
        matrix[100][100] = 0;
        assert(matrix.size() == 18);

    return 0;
}
