#include "tsplib.h"
#include <ctime>
#include <algorithm>
#include "../Little/Little.h"

using std::cout;
using std::endl;
using std::getline;
using std::find;
using std::stringstream;
using std::stoi;

// Called if there is only a input file, and no output file
Tsplib::Tsplib(ifstream& inputFile) {
    if (readProblem(inputFile)) {
        printSolution();
    }
}

// Called if there is a input and a ouput file
Tsplib::Tsplib(ifstream& inputFile, ofstream& outputFile) {
    if (readProblem(inputFile)) {
        writeSolution(outputFile);
    }
}

/*
 * Read the submitted TSPlib file, return false on error,
 * When parsing the TSP, return false on error
 * true otherwise
 */
bool Tsplib::readProblem(ifstream& inputFile) {
    string line;
    bool isMatrix = 0;
    while(inputFile) {
        getline(inputFile, line);
        if (line == "EOF" || line == "DISPLAY_DATA_SECTION") {
            break;
        }
        if (line.find(delimiter) != string::npos) {
            string keyword = line.substr(0, line.find(delimiter));
            string value = line.substr(line.find(delimiter) + 1, line.npos);
            
            if (!checkKeyword(trim(keyword), trim(value))) {
                return false;
            }
        }
        if (isMatrix) {
            stringstream stream(line);
            int n;
            while (stream >> n) {
                this->numbers.push_back(n);
            }
        }
        if (line == "EDGE_WEIGHT_SECTION") {
            isMatrix = true;
        }
    }
    
    if (!fillMatrix()) {
        return false;
    }
    
    Little<int> little(this->matrix);
    little.findTour();
    
    this->optimalTour = little.getLastTour();
    this->cost = little.getCost();
    
    return true;
}

// Check and store the value of each keyword
bool Tsplib::checkKeyword(string keyword, string value) {
    if (keyword == "NAME") {
        this->name = value;
    }
    else if (keyword == "TYPE") {
        if ((value == "TSP") || (value == "ATSP")) {
            this->type = value;
        }
        else {
            cout << keyword << " not supported" << endl;
            return 0;
        }
    }
    else if (keyword == "COMMENT") {
        this->comment = value;
    }
    else if (keyword == "DIMENSION") {
        this->dimension = stoi(value);
    }
    else if (keyword == "EDGE_WEIGHT_TYPE") {
        if (value == "EXPLICIT") {
            this->edgeWeightType = value;
        }
        else {
            cout << keyword << " not supported" << endl;
            return 0;
        }
    }
    else if (keyword == "EDGE_WEIGHT_FORMAT") {
        if ((value == "FULL_MATRIX")  || 
                (value == "UPPER_ROW") || 
                (value == "LOWER_ROW") || 
                (value == "UPPER_DIAG_ROW") || 
                (value == "LOWER_DIAG_ROW") || 
                (value == "UPPER_COL") || 
                (value == "LOWER_COL") || 
                (value == "UPPER_DIAG_COL") || 
                (value == "LOWER_DIAG_COL")) {
            this->edgeWeightFormat = value;
        }
        else {
            cout << keyword << " not supported" << endl;
            return 0;
        }
    }
    else if (keyword == "DISPLAY_DATA_TYPE") {
        
    }
    else {
        cout << "Error : Unknown keyword " << keyword << endl;
        return false;
    }
    return true;
}

string Tsplib::trim(string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// Dislay the solution on the standard output
void Tsplib::printSolution() {
    time_t now = time(0);
    tm* localtm = localtime(&now);
    
    cout << "NAME : " << this->name << "." << this->optimalTour.size() << ".tour" << endl;
    cout << "COMMENT : Lenght = " << this->cost << ". Found by John D.C. Little " << asctime(localtm);
    cout << "TYPE : TOUR" << endl;
    cout << "DIMENSION : " << this->optimalTour.size() << endl;
    cout << "TOUR_SECTION" << endl;
    
    for (int i : this->optimalTour) {
        cout << i << " ";
    }
    cout << "-1" << endl;
}

// Write the solution in the specified output
void Tsplib::writeSolution(ofstream& outputFile) {
    time_t now = time(0);
    tm* localtm = localtime(&now);
    
    outputFile << "NAME : " << this->name << "." << this->optimalTour.size() << ".tour" << endl;
    outputFile << "COMMENT : Lenght = " << this->cost << ". Found by John D.C. Little " << asctime(localtm);
    outputFile << "TYPE : TOUR" << endl;
    outputFile << "DIMENSION : " << this->optimalTour.size() << endl;
    outputFile << "TOUR_SECTION" << endl;
    
    for (int i : this->optimalTour) {
        outputFile << i << endl;
    }
    outputFile << "-1" << endl;
    outputFile << "EOF";
}

// Call the matrix parser function corresponding to the submitted matrix type
bool Tsplib::fillMatrix() {
    Matrix<int> matrix(this->dimension, this->dimension, 999999999);
    this->matrix = matrix;
    if (edgeWeightFormat == "FULL_MATRIX") {
        fullMatrix();
    }
    if ((edgeWeightFormat == "UPPER_ROW") or (edgeWeightFormat == "LOWER_COL")) {
        upperRow();
    }
    if ((edgeWeightFormat == "LOWER_ROW") or (edgeWeightFormat == "UPPER_COL")) {
        lowerRow();
    }
    if ((edgeWeightFormat == "UPPER_DIAG_ROW") or (edgeWeightFormat == "LOWER_DIAG_COL")) {
        upperDiagRow();
    }
    if ((edgeWeightFormat == "LOWER_DIAG_ROW") or (edgeWeightFormat == "UPPER_DIAG_COL")) {
        lowerDiagRow();
    }
    
    return true;
}

// Full matrix parser
void Tsplib::fullMatrix() {
    for (int i = 0; i < this->dimension; i++) {
        for (int j = 0; j < this->dimension; j++) {
            if (i != j) {
                matrix.setValue(i, j, this->numbers[i * this->dimension + j]);
            }
        }
    }
}

// Upper diagonal matrix without center diagonal parser
void Tsplib::upperRow(){
    int counter = 0;
    for (int i = 0; i < this->dimension - 1; i++) {
        for (int j = i + 1; j < this->dimension; j++) {
            matrix.setValue(i, j, this->numbers[counter]);
            matrix.setValue(j, i, this->numbers[counter]);
            counter++;
        }
    }
}

// Lower diagonal matrix without center diagonal parser
void Tsplib::lowerRow(){
    int counter = 0;
    for (int i = 1; i < this->dimension; i++) {
        for (int j = 0; j < i; j++) {
            matrix.setValue(i, j, this->numbers[counter]);
            matrix.setValue(j, i, this->numbers[counter]);
            counter++;
        }
    }
}

// Upper diagonal matrix parser
void Tsplib::upperDiagRow(){
    int counter = 0;
    for (int i = 0; i < this->dimension; i++) {
        for (int j = i; j < this->dimension; j++) {
            if (i != j) {
                matrix.setValue(i, j, this->numbers[counter]);
                matrix.setValue(j, i, this->numbers[counter]);
            }
            counter++;
        }
    }
}

// Lower diagonal matrix parser
void Tsplib::lowerDiagRow(){
    int counter = 0;
    for (int i = 0; i < this->dimension; i++) {
        for (int j = 0; j < i + 1; j++) {
            if (j != i) {
                matrix.setValue(i, j, this->numbers[counter]);
                matrix.setValue(j, i, this->numbers[counter]);
            }
            counter++;
        }
    }
}