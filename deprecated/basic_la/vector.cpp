#include "helpers.h"
#include "matrix.h"
#include "vector.h"

#include <cmath>
#include <ctime>

template <typename T>
TVector<T>::TVector() 
    : Data{nullptr}
    , Size{0}
    {

    }

template <typename T>
TVector<T>::TVector(size_t size) 
    : Size{size}
    {
        Data = new T[Size]; 
    }

template <typename T>
TVector<T>::TVector(const TVector& other) 
    : TVector<T>(other.Size)
    {
        for (size_t i = 0; i < Size; ++i) {
            Data[i] = other.Data[i];
        }
    }

template <typename T>
TVector<T>::TVector(TVector&& other) noexcept 
    : Size{other.Size} 
    {
        Data = other.Data;
        other.Data = nullptr;
    }

template <typename T>
TVector<T>::TVector(const TMatrix<T>& a, size_t j) 
    : Size{a.GetSize1()}
    {
        Data = new T[Size];
        for (size_t i = 0; i < Size; ++i) {
            Data[i] = a.Data[j * a.Size1 + i];
        }
    }

template <typename T>
TVector<T> TVector<T>::operator =(const TVector& other) {
    Size = other.Size;
    Data = new T[Size];
    for (size_t i = 0; i < Size; ++i) {
        Data[i] = other.Data[i];
    }
    return *this;
}

template <typename T>
TVector<T> TVector<T>::operator =(TVector&& other) noexcept {
    Size = other.Size;
    Data = other.Data;
    other.Data = nullptr;
    return *this;
}

template <typename T>
size_t TVector<T>::GetSize() const noexcept {
    return Size;
}

template <typename T>
void TVector<T>::Nullify() noexcept {
    if (!Data) {
        return;
    }
    for (size_t i = 0; i < Size; ++i) {
        Data[i] = {};
    }
}

template <typename T>
bool TVector<T>::operator !() const noexcept {
    return !Data;
}

template <typename T>
T& TVector<T>::operator ()(size_t i) const {
    return Data[i];
}

template <typename T>
TVector<T>& TVector<T>::operator +=(const TVector& other) {
    for (size_t i = 0; i < Size; ++i) {
        Data[i] += other.Data[i];
    }
    return *this;
}

template <typename T>
TVector<T>& TVector<T>::operator -=(const TVector& other) {
    for (size_t i = 0; i < Size; ++i) {
        Data[i] -= other(i);
    }
    return *this;
}

template <typename T> template <typename DType>
TVector<T>& TVector<T>::operator *=(const DType coeff) {
    for (size_t i = 0; i < Size; ++i) {
        Data[i] *= coeff;
    }
    return *this;
}

template <typename T>
double TVector<T>::Norm2(const TVector<T>& v) {
    double norm = 0;
    size_t size = v.Size;
    for (size_t i = 0; i < size; ++i) {
        norm += v(i) * v(i);
    }
    return std::sqrt(norm);
}

template <typename T>
double TVector<T>::InnerProd(const TVector<T>& v, const TVector<T>& u) {
    double res = 0.0;
    for (size_t i = 0; i < v.Size; ++i) {
        res += v(i) * u(i);
    }
    return res;
}

template <typename T>
TVector<T> TVector<T>::CreateRandom(const size_t size) {
    std::srand(std::time(nullptr));
    TVector<T> res(size);
    for (size_t i = 0; i < size; ++i) {
        res.Data[i] = std::rand() % (size * size);
    }
    return res;
}

template <typename T>
TVector<T>::~TVector() {
    if (Data) {
        delete [] Data;
        Data = nullptr;
    }
    Size = 0;
}


template <typename T>
std::ostream& operator <<(std::ostream& out, const TVector<T>& v) {
    size_t size = v.GetSize();
    for (size_t i = 0; i < size; ++i) {
        out << v(i) << ' ';
    }
    return out;
}

template <typename T>
bool operator ==(const TVector<T>& a, const TVector<T>& b) {
    size_t size = a.GetSize();
    for (size_t i = 0; i < size; ++i) {
        if (!RoughEq(a(i), b(i))) {
            return false;
        }
    }
    return true;
}

template <typename T>
bool operator !=(const TVector<T>& a, const TVector<T>& b) {
    return !(a == b);
}

template <typename T>
TVector<T> operator +(const TVector<T>& a, const TVector<T>& b) {
    TVector<T> res(a);
    return res += b;
}

template <typename T>
TVector<T> operator -(const TVector<T>& a, const TVector<T>& b) {
    TVector<T> res(a);
    return res -= b;
}

template <typename T1, typename T2>
TVector<T1> operator *(const TVector<T1>& a, const T2 coeff) {
    TVector<T1> res(a);
    return res *= coeff;
}

template <typename T>
T InnerProd(const TVector<T>& a, const TVector<T>& b) {
    T res{};
    size_t size = a.Size;
    for (size_t i = 0; i < size; ++i) {
        res += a[i] * b[i];
    }
}


template <> 
double TVector<TMatrix<double>>::Norm2(const TVector<TMatrix<double>>& v) = delete;

template <>
double TVector<TMatrix<double>>::InnerProd(const TVector<TMatrix<double>>& v, const TVector<TMatrix<double>>& u) = delete;

template std::ostream& operator <<(std::ostream& out, const TVector<double>& v);
template bool operator ==(const TVector<double>& a, const TVector<double>& b);
template bool operator !=(const TVector<double>& a, const TVector<double>& b);
template TVector<double> operator +(const TVector<double>& a, const TVector<double>& b);
template TVector<double> operator -(const TVector<double>& a, const TVector<double>& b);
template TVector<double> operator *(const TVector<double>& a, const double coeff);

template class TVector<double>;
template class TVector<TMatrix<double>>;