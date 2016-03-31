#ifndef TSPLIB_H
#define	TSPLIB_H

#include <string>
#include <iostream>
#include <fstream>
#include "../Matrix/Matrix.h"

using std::string;
using std::ifstream;
using std::ofstream;

class Tsplib {
private:
    const char delimiter = ':';

    string name;
    string type;
    string comment;
    int dimension;
    string edgeWeightType;
    string edgeWeightFormat;
    
    vector<int> numbers;    // Submitted matrix in the TSP file, on only one line
    Matrix<int> matrix;     // Interpreted Matrix, usable for the Little algorithm
    vector<int> optimalTour;    // Optimal tour found thanks to the Little algorithm
    int cost;   // Cost of the found tour
    
    bool checkKeyword(string, string);
    string trim(string);
    bool readProblem(ifstream&);
    void printSolution();
    void writeSolution(ofstream&);
    bool fillMatrix();
    void fullMatrix();
    void upperRow();
    void lowerRow();
    void upperDiagRow();
    void lowerDiagRow();
    
public:
    Tsplib(ifstream&);
    Tsplib(ifstream&, ofstream&);
};

#endif	/* TSPLIB_H */