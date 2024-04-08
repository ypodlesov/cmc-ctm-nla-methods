#include "helpers.h"
#include "matrix.h"
#include "vector.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <future>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

template <typename T>
TMatrix<T>::TMatrix()
    : Data{nullptr}
    , Size1{0}
    , Size2{0}
    {
    }

template <typename T>
TMatrix<T>::TMatrix(size_t size1, size_t size2)
    : Size1{size1}
    , Size2{size2}
    {
        Data = new T[Size1 * Size2];
    }

template <typename T>
TMatrix<T>::TMatrix(size_t size)
    : TMatrix(size, size)
    {
    }

template <typename T>
TMatrix<T>::TMatrix(const TMatrix& other) 
    : TMatrix<T>(other.Size1, other.Size2)
    {
        for (size_t i = 0; i < Size1 * Size2; ++i) {
            Data[i] = other.Data[i];
        }
    }

template <typename T>
TMatrix<T>::TMatrix(TMatrix&& other) noexcept 
    : Size1{other.Size1}
    , Size2{other.Size2}
    {
        Data = other.Data;
        other.Data = nullptr;
    }

template <typename T>
TMatrix<T>& TMatrix<T>::operator =(const TMatrix& other) {
    Size1 = other.Size1;
    Size2 = other.Size2;
    Data = new T[Size1 * Size2];
    for (size_t i = 0; i < Size1 * Size2; ++i) {
        Data[i] = other.Data[i];
    }
    return *this;
}

template <typename T>
TMatrix<T>& TMatrix<T>::operator =(TMatrix&& other) noexcept {
    Size1 = other.Size1;
    Size2 = other.Size2;
    Data = other.Data;
    other.Data = nullptr;
    return *this;
}

template <typename T>
size_t TMatrix<T>::GetSize1() const noexcept {
    return Size1;
}

template <typename T>
size_t TMatrix<T>::GetSize2() const noexcept {
    return Size2;
}

template <typename T>
bool TMatrix<T>::operator !() const noexcept {
    return !Data;
}

template <typename T>
T& TMatrix<T>::operator ()(size_t i, size_t j) const {
    return Data[j * Size1 + i];
}

template <typename T>
TMatrix<T>& TMatrix<T>::operator +=(const TMatrix<T>& other) {
    for (size_t i = 0; i < Size1 * Size2; ++i) {
        Data[i] += other.Data[i];
    }
    return *this;
}

template <typename T>
TMatrix<T>& TMatrix<T>::operator -=(const TMatrix<T>& other) {
    for (size_t i = 0; i < Size1 * Size2; ++i) {
        Data[i] -= other.Data[i];
    }
    return *this;
}

template <typename T> template <typename DType>
TMatrix<T>& TMatrix<T>::operator *=(const DType coeff) {
    for (size_t i = 0; i < Size1 * Size2; ++i) {
        Data[i] *= coeff;
    }
    return *this;
}

template <typename T>
TMatrix<T>::operator double() const {
    return Norm2();
}

template <typename T>
void TMatrix<T>::Nullify() {
    if (!Data) {
        return;
    }
    for (size_t i = 0; i < Size1 * Size2; ++i) {
        Data[i] = {};
    }
}

template <typename T>
TMatrix<T> TMatrix<T>::Transpose() const {
    TMatrix<T> res(Size2, Size1);
    for (size_t i = 0; i < Size1; ++i) {
        for (size_t j = 0; j < Size2; ++j) {
            res.Data[i * Size2 + j] = Data[j * Size1 + i];
        }
    }
    return res;
}

template <typename T>
TMatrix<T> TMatrix<T>::ConstructFromRange(size_t row1, size_t row2, size_t col1, size_t col2) const { // [row1,row2), [col1, col2)
    TMatrix<T> res(row2 - row1, col2 - col1);
    for (size_t i = row1; i < row2; ++i) {
        for (size_t j = col1; j < col2; ++j) {
            res(i - row1, j - col1) = Data[j * Size1 + i];
        }
    }
    return res;
}

template <typename T>
bool TMatrix<T>::IsTriangular(const TMatrix<T>& a, ETriangularType type) {
    bool isUpper = type == ETriangularType::Upper;
    bool flag = true;
    for (size_t i = 0; i < a.GetSize1(); ++i) {
        for (size_t j = (isUpper ? 0 : i + 1); j < (isUpper ? i : a.GetSize2()); ++j) {
            flag &= RoughEq(a(i, j), 0.0);
        }
    }
    return flag;
}

template <typename T>
double TMatrix<T>::Norm2() const {
    double res = 0;
    for (size_t i = 0; i < Size1; ++i) {
        for (size_t j = 0; j < Size2; ++j) {
            res += Data[j * Size1 + i];
        }
    }
    return std::sqrt(res);
}

template <typename T>
double TMatrix<T>::ColumnNorm2(size_t j) const {
    double norm = 0;
    for (size_t i = Size1 * j; i < Size1 * (j + 1); ++i) {
        norm += Data[i] * Data[i];
    }
    return std::sqrt(norm);
}

template <typename T>
bool TMatrix<T>::AbleToMultiply(const TMatrix<T>& a, const TMatrix<T>& b) {
    return a.GetSize2() == b.GetSize1();
}

template <typename T>
TMatrix<TMatrix<T>> TMatrix<T>::ConstructBlockMatrix(const std::pair<size_t, size_t>& blockSize) const {
    size_t blockSize1 = blockSize.first;
    size_t blockSize2 = blockSize.second;
    size_t blockMatrixSize1 = (Size1 +  blockSize1 - 1) / blockSize1;
    size_t blockMatrixSize2 = (Size2 +  blockSize2 - 1) / blockSize2;
    TMatrix<TMatrix<T>> blockMatrix(blockMatrixSize1, blockMatrixSize2);
    for (size_t j = 0; j < Size2; j += blockSize2) {
        for (size_t i = 0; i < Size1; i += blockSize1) {
            blockMatrix(i / blockSize1, j / blockSize2) = ConstructFromRange(i, std::min(i + blockSize1, Size1), j, std::min(j + blockSize2, Size2));
        }
    }
    return blockMatrix;
}

template <typename T>
T* TMatrix<T>::GetColumn(const size_t j) const {
    return &Data[j * Size1];
}

template <typename T>
TMatrix<T> TMatrix<T>::MMMultiply(const TMatrix<T>& a, const TMatrix<T>& b) {
    if (!AbleToMultiply(a, b)) {
        return TMatrix<T>();
    }
    TMatrix<T> res(a.GetSize1(), b.GetSize2());
    for (size_t i = 0; i < a.GetSize1(); ++i) {
        for (size_t j = 0; j < b.GetSize2(); ++j) {
            res(i, j) = 0;
            for (size_t k = 0; k < a.GetSize2(); ++k) {
                res(i, j) += a(i, k) * b(k, j);
            }
        }
    }
    return res;
}

template <typename T>
TMatrix<T> TMatrix<T>::CreateIdentityMatrix(const size_t n) {
    TMatrix<T> I(n);
    I.Nullify();
    for (size_t i = 0; i < n; ++i) {
        I(i, i) = 1;
    }
    return I;
}

template <typename T>
TMatrix<T> TMatrix<T>::CreateRandom(const size_t size1, const size_t size2) {
    std::srand(std::time(nullptr));
    TMatrix<T> res(size1, size2);
    for (size_t i = 0; i < size1; ++i) {
        for (size_t j = 0; j < size2; ++j) {
            res(i, j) = std::rand() % (size1 * size2);
        }
    }
    return res;
}

template <typename T>
T TMatrix<T>::InnerProd(const TMatrix<T>& a, size_t a_column, const TMatrix<T>& b, size_t b_column) {
    T result{};
    for (size_t i = 0; i < a.Size1; ++i) {
        result += a.Data[a_column * a.Size1 + i] * b.Data[b_column * b.Size1 + i];
    }
    return result;
}

template <typename T>
TMatrix<T> TMatrix<T>::GenerateRandomTriangular(const size_t m, const size_t n, ETriangularType type) {
    std::srand(std::time(nullptr));
    TMatrix<T> result(m, n);
    T* data = result.Data;
    for (size_t j = 0; j < n; ++j) {
        for (size_t i = 0; i <= j; ++i) {
            data[m * j + i] = std::rand() % (m * n);
        }
    }
    return result;
}

template <typename T>
TMatrix<T> TMatrix<T>::GenerateRandomSymmetricDefinite(const size_t n, EDefiniteness definiteness) {
    std::srand(std::time(nullptr));
    TMatrix<T> result = GenerateRandomTriangular(n, n, ETriangularType::Upper);
    T* data = result.Data;
    for (size_t j = 0; j < n; ++j) {
        for (size_t i = 0; i < n; ++i) {
            if (i != j) {
                data[j * n + i] = data[i * n + j];
                continue;
            }
            switch (definiteness) {
                case EDefiniteness::Negative: {
                    data[n * i + i] = -double(std::abs(std::rand()) % (result.Size1 * result.Size2)) - 1;
                }
                case EDefiniteness::Positive: {
                    data[n * i + i] = double(std::abs(std::rand()) % (result.Size1 * result.Size2)) + 1;
                }
                case EDefiniteness::NonNegative: {
                    data[n * i + i] = double(std::abs(std::rand()) % (result.Size1 * result.Size2));
                }
                case EDefiniteness::NonPositive: {
                    data[n * i + i] = -double(std::abs(std::rand()) % (result.Size1 * result.Size2));
                }
            }
        }
    }
    return result;
}

template <typename T>
bool TMatrix<T>::MVMultiply(const TMatrix<T>& a, const TVector<T>& x, TVector<T>& result) {
    if (!a || !x || a.Size2 != x.Size) {
        return false;
    }
    if (!result || result.Size != a.Size1) {
        result = TVector<T>(a.Size1);
    }
    result.Nullify();
    T* aData = a.Data;
    T* xData = x.Data;
    T* resultData = result.Data;
    for (size_t j = 0; j < a.Size2; ++j) {
        size_t columnShift = j * a.Size1;
        T xj = xData[j];
        for (size_t i = 0; i < a.Size1; ++i) {
            resultData[i] += aData[columnShift + i] * xj;
        }
    }
    return true;
}

template <typename T>
TMatrix<T> TMatrix<T>::FromBlockMatrix(const TMatrix<TMatrix<T>>& a) {
    size_t n = 0, m = 0;
    for (size_t i = 0; i < a.GetSize1(); ++i) {
        n += a(i, 0).GetSize1();
    }
    for (size_t j = 0; j < a.GetSize2(); ++j) {
        m += a(0, j).GetSize2();
    }
    TMatrix<T> res(n, m);
    size_t i = 0;
    size_t j = 0;
    for (size_t ii = 0; ii < a.GetSize1(); ++ii) {
        for (size_t jj = 0; jj < a.GetSize2(); ++jj) {
            for (size_t cur_i = i; cur_i - i < a(ii, jj).GetSize1(); ++cur_i) {
                for (size_t cur_j = j; cur_j - j < a(ii, jj).GetSize2(); ++cur_j) {
                    res(cur_i, cur_j) = a(ii, jj)(cur_i - i, cur_j - j);
                }
            }
            j += a(ii, jj).GetSize2();
            if (jj == a.GetSize2() - 1) {
                i += a(ii, jj).GetSize1();
                j = 0;
            }
        }   
    }
    return res;
}


template <typename T> 
TMatrix<T>::~TMatrix() {
    if (Data != nullptr) {
        delete[] Data;
        Data = nullptr;
    }
    Size1 = Size2 = 0;
}

template <typename T>
std::ostream& operator <<(std::ostream& out, const TMatrix<T>& matrix) {
    for (size_t i = 0; i < matrix.GetSize1(); ++i) {
        for (size_t j = 0; j < matrix.GetSize2(); ++j) {
            out << matrix(i, j) << ' ';
        }
        out << '\n';
    }
    out <<std::endl;
    return out;
}

template <typename T>
bool operator ==(const TMatrix<T>& a, const TMatrix<T>& b) {
    for (size_t i = 0; i < a.GetSize1(); ++i) {
        for (size_t j = 0; j < a.GetSize2(); ++j) {
            if (!RoughEq(a(i, j), b(i, j))) {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
bool operator !=(const TMatrix<T>& a, const TMatrix<T>& b) {
    return !(a == b);
}

template <typename T>
TMatrix<T> operator+(const TMatrix<T>& a, const TMatrix<T>& b) {
    TMatrix<T> res(a);
    return res += b;
}

template <typename T>
TMatrix<T> operator -(const TMatrix<T>& a, const TMatrix<T>& b) {
    TMatrix<T> res(a);
    return res -= b;
}

template <typename T>
TMatrix<T> operator *(const TMatrix<T>& a, const TMatrix<T>& b) {
    return TMatrix<T>::MMMultiply(a, b); 
}

template <typename T1, typename T2>
TMatrix<T1> operator *(const TMatrix<T1>& a, const T2 coeff) {
    TMatrix<T1> res(a);
    return res *= coeff;
}

template std::ostream& operator <<(std::ostream& out, const TMatrix<double>& matrix);
template bool operator ==(const TMatrix<double>& a, const TMatrix<double>& b);
template bool operator !=(const TMatrix<double>& a, const TMatrix<double>& b);
template TMatrix<double> operator +(const TMatrix<double>& a, const TMatrix<double>& b);
template TMatrix<double> operator -(const TMatrix<double>& a, const TMatrix<double>& b);
template TMatrix<double> operator *(const TMatrix<double>& a, const TMatrix<double>& b);
template TMatrix<double> operator *(const TMatrix<double>& a, const double coeff);

template std::ostream& operator <<(std::ostream& out, const TMatrix<TMatrix<double>>& matrix);
template bool operator ==(const TMatrix<TMatrix<double>>& a, const TMatrix<TMatrix<double>>& b);
template bool operator !=(const TMatrix<TMatrix<double>>& a, const TMatrix<TMatrix<double>>& b);
template TMatrix<TMatrix<double>> operator +(const TMatrix<TMatrix<double>>& a, const TMatrix<TMatrix<double>>& b);
template TMatrix<TMatrix<double>> operator -(const TMatrix<TMatrix<double>>& a, const TMatrix<TMatrix<double>>& b);
template TMatrix<TMatrix<double>> operator *(const TMatrix<TMatrix<double>>& a, const TMatrix<TMatrix<double>>& b);
template TMatrix<TMatrix<double>> operator *(const TMatrix<TMatrix<double>>& a, const double coeff);

template class TMatrix<double>;
template class TMatrix<TMatrix<double>>;