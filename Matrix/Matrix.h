#ifndef MATRIX_H
#define	MATRIX_H

#include "IndexOutOfBoundsException.h"
#include "NegativeDimensionException.h"
#include <iostream>
#include <vector>

using std::vector;
using std::ostream;

template<class T> class Matrix;
template<class T> ostream& operator<<(ostream&, Matrix<T>&);

template<class T> class Matrix {
private:
    int row;
    int col;
    T emptyVal;
    vector<vector<T> > data;
    
public:
    Matrix(int nbRows = 0, int nbColumns = 0, T emptyValue = 0) throw(NegativeDimensionException);

    T getEmptyValue() { return this->emptyVal; }

    int getNbRows() { return this->row; }
    void addRow(int rowIndex) throw(IndexOutOfBoundsException);
    void removeRow(int rowIndex) throw(IndexOutOfBoundsException);

    int getNbColumns() { return this->col; }
    void addColumn(int colIndex) throw(IndexOutOfBoundsException);
    void removeColumn(int colIndex) throw(IndexOutOfBoundsException);

    T getValue(int rowIndex, int colIndex) throw(IndexOutOfBoundsException);
    void setValue(int rowIndex, int colIndex, T value) throw(IndexOutOfBoundsException);

    ~Matrix();
    
    friend ostream& operator<< <>(ostream&, Matrix<T>&);
};

template <class T> ostream& operator<<(ostream& stream, Matrix<T>& matrix) {
    for (int i = 0; i < matrix.row; i++) {
        for (int j = 0; j < matrix.col; j++) {
            if (matrix.getValue(i, j) == matrix.getEmptyValue()) {
                stream.width(5);
                stream << "/  ";
            }
            else {
                stream.width(4);
                stream << matrix.getValue(i, j) << " ";
            }
        }
        stream << std::endl;
    }
    return stream;
}

template <class T> Matrix<T>::Matrix(int nbRows, int nbColumns, T emptyValue) throw(NegativeDimensionException){
    if (nbColumns < 0 or nbRows < 0) {
        throw NegativeDimensionException();
    }
    this->data = vector<vector<T> >(nbRows, std::vector<T>(nbColumns, emptyValue));
    this->row = nbRows;
    this->col = nbColumns;
    this->emptyVal = emptyValue;
}

template <class T> Matrix<T>::~Matrix() {

}

template <class T> void Matrix<T>::addRow(int rowIndex) throw(IndexOutOfBoundsException) {
    if (rowIndex < 0 or rowIndex > this->row) {
        throw IndexOutOfBoundsException(rowIndex, 0, this->row);
    }
    typename vector<vector<T> >::iterator it = this->data.begin() + rowIndex;
    this->data.insert(it, vector<T>(this->col, this->emptyVal));
    this->row++;
}

template <class T> void Matrix<T>::removeRow(int rowIndex) throw(IndexOutOfBoundsException) {
    if (rowIndex < 0 or rowIndex >= this->row) {
        throw IndexOutOfBoundsException(rowIndex, 0, this->row - 1);
    }
    typename vector<vector<T> >::iterator it = this->data.begin() + rowIndex;
    this->data.erase(it);
    this->row--;
}

template <class T> void Matrix<T>::addColumn(int colIndex) throw(IndexOutOfBoundsException) {
    if (colIndex < 0 or colIndex > this->col) {
        throw IndexOutOfBoundsException(colIndex, 0, this->col);
    }
    for (int row = 0; row < this->row; row++) {
        typename vector<T>::iterator it = this->data[row].begin() + colIndex;
        this->data[row].insert(it, this->emptyVal);
    }
    this->col++;
}

template <class T> void Matrix<T>::removeColumn(int colIndex) throw(IndexOutOfBoundsException) {
    if (colIndex < 0 or colIndex >= this->col) {
        throw IndexOutOfBoundsException(colIndex, 0, this->col - 1);
    }
    for (int row = 0; row < this->row; row++) {
        typename vector<T>::iterator it = this->data[row].begin() + colIndex;
        this->data[row].erase(it);
    }
    this->col--;
}

template <class T> T Matrix<T>::getValue(int rowIndex, int colIndex) throw(IndexOutOfBoundsException) {
    if (rowIndex < 0 or rowIndex >= this->row) {
        throw IndexOutOfBoundsException(rowIndex, 0, this->row - 1);
    }
    if (colIndex < 0 or colIndex >= this->col) {
        throw IndexOutOfBoundsException(colIndex, 0, this->col - 1);;
    }
    return this->data[rowIndex][colIndex];
}

template <class T> void Matrix<T>::setValue(int rowIndex, int colIndex, T value) throw(IndexOutOfBoundsException) {
    if (rowIndex < 0 or rowIndex >= this->row) {
        throw IndexOutOfBoundsException(rowIndex, 0, this->row - 1);
    }
    if (colIndex < 0 or colIndex >= this->col) {
        throw IndexOutOfBoundsException(colIndex, 0, this->col - 1);
    }
    this->data[rowIndex][colIndex] = value;
}

#endif	/* MATRIX_H */