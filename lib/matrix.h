#pragma once
#include <vector>
#include <iostream>
#include <QDebug>
using namespace std;

template<typename T>
class Matrix
{
public:
    //Constructors
    Matrix();
    Matrix(int row, int column);
    Matrix(int row, int column, T val);
    Matrix(Matrix const& secondMatrix);
    virtual ~Matrix();   //Destructor shoule be declared as virtual unless it won't be inheritanced

    //Basic operation
    int getRow() const;
    int getColumn() const;
    T getValue(int x, int y) const;
    void setValue(int x, int y,T val);
    void show();

    //Math operation
    Matrix add(Matrix const& secondMatrix) const;
    Matrix subtract(Matrix const& secondMatrix) const;
    Matrix multiply(Matrix const& secondMatrix) const;

    //Operator overloading
    Matrix& operator=(Matrix const& secondMatrix);
    Matrix operator+(Matrix const& secondMatrix) const;
    Matrix operator-(Matrix const& secondMatrix) const;
    Matrix operator*(Matrix const& secondMatrix) const;
    Matrix operator+=(Matrix const& secondMatrix);
    Matrix operator-=(Matrix const& secondMatrix);
    Matrix operator*=(Matrix const& secondMatrix);
    T* operator[](int x) const;

private:
    int row;
    int column;
    T **value;

    void initialize();     //Initialize Matrix & allocate memory
};

/********************************************************************************/

template<typename T>
Matrix<T>::Matrix() {
    this->row = 1;
    this->column = 1;
    initialize();
    value[0][0] = 0;
}

template<typename T>
Matrix<T>::Matrix(int row, int column) {
    this->row = row;
    this->column = column;
    initialize();
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            value[i][j] = 0;
        }
    }
}

template<typename T>
Matrix<T>::Matrix(int row, int column, T val) {
    this->row = row;
    this->column = column;
    initialize();
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            value[i][j] = val;
        }
    }
}

template<typename T>
Matrix<T>::Matrix(Matrix const& secondMatrix) {
    this->row = secondMatrix.getRow();
    this->column = secondMatrix.getColumn();
    initialize();
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            value[i][j] = secondMatrix.getValue(i, j);
        }
    }
}

template<typename T>
Matrix<T>::~Matrix() {
    for (int i = 0; i < row; i++) {
        delete[] value[i];
    }
    delete[] value;
}

template<typename T>
void Matrix<T>::initialize() {
    value = new T * [row];
    for (int i = 0; i < row; i++) {
        value[i] = new T[column];
    }
}

template<typename T>
int Matrix<T>::getRow() const {
    return this->row;
}

template<typename T>
int Matrix<T>::getColumn() const {
    return this->column;
}

template<typename T>
T Matrix<T>::getValue(int x, int y) const {
    if(x >= getRow()||y >= getColumn())
        return 0;
    return value[x][y];
}

template<typename T>
void Matrix<T>::setValue(int x, int y, T val) {
    if(x >= getRow()||y >= getColumn())
        return ;
    value[x][y] = val;
}

template<typename T>
void Matrix<T>::show() {
    for (int i = 0; i < getRow(); i++) {
        for (int j = 0; j < getColumn(); j++) {
            cout << getValue(i, j) << ",";
        }
        cout << endl;
    }
    cout << endl;
}

template<typename T>
Matrix<T> Matrix<T>::add(Matrix const& secondMatrix) const {
    Matrix result(row, column);
    if (secondMatrix.getRow() != getRow()
        || secondMatrix.getColumn() != getColumn()) {
        cout << "Sizes of Two Matrix are not equal! " << endl;
        return *this;
    }
    for (int i = 0; i < getRow(); i++) {
        for (int j = 0; j < getColumn(); j++) {
            result.setValue(i, j, value[i][j] + secondMatrix.getValue(i, j));
        }
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::subtract(Matrix const& secondMatrix) const {
    Matrix result(row, column);
    if (secondMatrix.getRow() != getRow()
        || secondMatrix.getColumn() != getColumn()) {
        cout << "Sizes of Two Matrix are not equal! " << endl;
        return *this;
    }
    for (int i = 0; i < getRow(); i++) {
        for (int j = 0; j < getColumn(); j++) {
            result.setValue(i, j, value[i][j] - secondMatrix.getValue(i, j));
        }
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::multiply(Matrix const& secondMatrix) const {
    Matrix<T> result(row, secondMatrix.getColumn(), 0);
    if (getColumn() != secondMatrix.getRow()) {
        //qDebug() << "Wrong Format! ";
        return *this;
    }
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < secondMatrix.getColumn(); j++) {
            T sum = 0;
            for (int k = 0; k < secondMatrix.getRow(); k++) {
                sum += value[i][k] * secondMatrix.getValue(k, j);
            }
            result.setValue(i, j, sum);
        }
    }

    return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix const& secondMatrix) {
    if (this == &secondMatrix)
        return *this;
    if (secondMatrix.getRow() != getRow()
        || secondMatrix.getColumn() != getColumn()) {
        for (int i = 0; i < row; i++) {
            delete[] value[i];
        }
        delete[] value;

        row = secondMatrix.getRow();
        column = secondMatrix.getColumn();
        initialize();
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            value[i][j] = secondMatrix.getValue(i, j);
        }
    }
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(Matrix const& secondMatrix) const {
    return add(secondMatrix);
}

template<typename T>
Matrix<T> Matrix<T>::operator-(Matrix const& secondMatrix) const {
    return subtract(secondMatrix);
}

template<typename T>
Matrix<T> Matrix<T>::operator*(Matrix const& secondMatrix) const {
    return multiply(secondMatrix);
}

template<typename T>
Matrix<T> Matrix<T>::operator+=(Matrix const& secondMatrix) {
    *this = add(secondMatrix);
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator-=(Matrix const& secondMatrix) {
    *this = subtract(secondMatrix);
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator*=(Matrix const& secondMatrix) {
    *this = multiply(secondMatrix);
    return *this;
}

template<typename T>
T* Matrix<T>::operator[](int x) const {
    return value[x];
}
