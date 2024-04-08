#pragma once
#include <iostream>

enum class ETriangularType {
    Upper,
    Lower
};

enum class EDefiniteness {
    Positive,
    Negative,
    NonPositive,
    NonNegative
};

template <typename T>
class TVector;

template <typename T>
class TMatrix {
public:
    TMatrix();
    TMatrix(size_t size1, size_t size2);
    TMatrix(size_t size);
    TMatrix(const TMatrix& other);
    TMatrix(TMatrix&& other) noexcept;
    TMatrix& operator =(const TMatrix& other);
    TMatrix& operator =(TMatrix&& other) noexcept;

    size_t GetSize1() const noexcept;
    size_t GetSize2() const noexcept;

    bool operator !() const noexcept;
    T& operator ()(size_t row, size_t column) const;
    TMatrix& operator +=(const TMatrix& other);
    TMatrix& operator -=(const TMatrix& other);
    template <typename DType>
    TMatrix& operator *=(const DType coeff);
    operator double() const;

    void Nullify();
    TMatrix Transpose() const;
    TMatrix ConstructFromRange(size_t row1, size_t row2, size_t col1, size_t col2) const; // [row1,row2), [col1, col2)
    double ColumnNorm2(size_t colNum) const;
    double Norm2() const;
    TMatrix<TMatrix> ConstructBlockMatrix(const std::pair<size_t, size_t>& blockSize) const;
    T* GetColumn(const size_t j) const;

    static bool IsTriangular(const TMatrix& a, ETriangularType type);
    static bool AbleToMultiply(const TMatrix& a, const TMatrix& b);
    static TMatrix MMMultiply(const TMatrix& a, const TMatrix& b);
    static TMatrix CreateIdentityMatrix(const size_t n);
    static TMatrix CreateRandom(const size_t size1, const size_t size2);
    static T InnerProd(const TMatrix& a, size_t a_column, const TMatrix& b, size_t b_column);
    static TMatrix FromBlockMatrix(const TMatrix<TMatrix>& a);
    static TMatrix GenerateRandomTriangular(const size_t m, const size_t n, ETriangularType type);
    static TMatrix GenerateRandomSymmetricDefinite(const size_t n, EDefiniteness definiteness);
    static bool MVMultiply(const TMatrix& a, const TVector<T>& x, TVector<T>& result);

    friend bool QRDecomposition(TMatrix<double>& Q, TMatrix<double>& R);
    friend bool QRDecompositionBlockOptimized(TMatrix<double>& Q, TMatrix<double>& R);
    friend bool ConjugateGradient(const TMatrix<double>& A, const TVector<double>& b, TVector<double>& x);

    friend class TVector<T>;

    ~TMatrix();
    
private:
    size_t Size1{};
    size_t Size2{};
    T* Data{nullptr};
};

template <typename T>
std::ostream& operator <<(std::ostream& out, const TMatrix<T>& matrix);

template <typename T>
bool operator ==(const TMatrix<T>& a, const TMatrix<T>& b);

template <typename T>
bool operator !=(const TMatrix<T>& a, const TMatrix<T>& b);

template <typename T>
TMatrix<T> operator +(const TMatrix<T>& a, const TMatrix<T>& b);

template <typename T>
TMatrix<T> operator -(const TMatrix<T>& a, const TMatrix<T>& b);

template <typename T1, typename T2>
TMatrix<T1> operator *(const TMatrix<T1>& a, const TMatrix<T2>& b);

template <typename T1, typename T2>
TMatrix<T1> operator *(const TMatrix<T1>& a, const T2 coeff);