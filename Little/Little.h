#ifndef LITTLE_H
#define LITTLE_H

#include "../Matrix/Matrix.h"
#include <iostream>
#include <limits>
#include <deque>
#include <stack>
#include <utility>

using std::stack;
using std::deque;
using std::pair;
using std::cout;
using std::endl;

// Node structure
template<class T>
struct Node {
    T cost;                     // path cost till this node
    pair<int, int> path;        // path segment (example : 0 1)
    bool bar = false;           // included or excluded path segment
    long parentNodeKey = -1;     // parent id node (in tree)
};

template<class T>
class Little {
private:
    T infinity;                                     // value considered as infinity
    Matrix<T> initialMatrix;                        // initial matrix
    T reference = std::numeric_limits<T>::max();    // smallest cost found
    deque<Node<T> > tree;                           // tree storing the nodes
    vector<int> lastTour;                           // last found tour
    bool optimal = 0;                               // optimal path or not
    T getMinRow(Matrix<T> &m, int row, int ignoredCol = -1);
    T getMinCol(Matrix<T> &m, int col, int ignoredRow = -1);
    T reduceRow(Matrix<T> &m, int row);
    T reduceCol(Matrix<T> &m, int col);
    T reduceMatrix(Matrix<T> &m);
    T calculateRegret(Matrix<T> &m, pair<int, int> &path, pair<int, int> &pos);
    void removeSubTour(Matrix<T> &m, int index, pair<int, int> &path);
    void addIndices(Matrix<T> &m);
    vector<int> orderPath(int index, int begin);
    void addLastPath(Matrix<T> &m);
    void checkTourCost();

public:
    Little(Matrix<T> &m);
    void findTour();
    vector<int> getLastTour() { return this->lastTour; }    // Return the last found tour
    int getCost() { return this->reference; }               // Return the last found tour cost
    bool isOptimal() { return this->optimal; }              // Return whether the tour is optimal
};

// Return the minimum of a row in a matrix
template<class T> T Little<T>::getMinRow(Matrix<T> &m, int row, int ignoredCol) {
    int nbCol = m.getNbColumns();
    T min = this->infinity;
    for (int i = 1; i < nbCol; i++) {
        T currentValue = m.getValue(row, i);
        if (currentValue != this->infinity && i != ignoredCol) {
            min = (min < currentValue ? min : currentValue);
        }
    }
    return min;
};

// Return the minimum of a column in a matrix
template<class T> T Little<T>::getMinCol(Matrix<T> &m, int col, int ignoredRow) {
    int nbRow = m.getNbRows();
    T min = this->infinity;
    for (int i = 1; i < nbRow; i++) {
        T currentValue = m.getValue(i, col);
        if (currentValue != this->infinity && i != ignoredRow) {
            min = (min < currentValue ? min : currentValue);
        }
    }
    return min;
};

// Reduce a row of the matrix
template<class T> T Little<T>::reduceRow(Matrix<T> &m, int row) {
    int nbCol = m.getNbColumns();
    T min = getMinRow(m, row);
    for (int i = 1; i < nbCol; i++) {
        if (m.getValue(row, i) != this->infinity) {
            m.setValue(row, i, m.getValue(row, i) - min);
        }
    }
    return min;
};

// Reduce a column of the matrix
template<class T> T Little<T>::reduceCol(Matrix<T> &m, int col) {
    int nbRow = m.getNbRows();
    T min = getMinCol(m, col);
    for (int i = 1; i < nbRow; i++) {
        if (m.getValue(i, col) != this->infinity) {
            m.setValue(i, col, m.getValue(i, col) - min);
        }
    }
    return min;
};

/*
 * Reduce the matrix and return the sum of
 * the subtracted cost on each raw and each columns
 */
template<class T> T Little<T>::reduceMatrix(Matrix<T> &m) {
    int nbRow = m.getNbRows();
    T minRowTotal = 0;
    for (int i = 1; i < nbRow; i++) {
        minRowTotal += reduceRow(m, i);
    }

    int nbCol = m.getNbColumns();
    T minColTotal = 0;
    for (int i = 1; i < nbCol; i++) {
        minColTotal += reduceCol(m, i);
    }

    return minRowTotal + minColTotal;
};

/*
 * Return the path segment and the cell position
 * in the matrix containing the maximal regret
 */
template<class T> T Little<T>::calculateRegret(Matrix<T> &m, pair<int, int> &path, pair<int, int> &pos) {
    int size = m.getNbRows();
    int max = -1;
    for (int i = 1; i < size; i++) {
        for (int j = 1; j < size; j++) {
            if (m.getValue(i, j) == 0) {
                T val = getMinRow(m, i, j) + getMinCol(m, j, i);
                if (max < val || max < 0) {
                    max = val;
                    pos.first = i;
                    pos.second = j;

                    path.first = m.getValue(i, 0);
                    path.second = m.getValue(0, j);
                }
            }
        }
    }
    return max;
};

// Search and remove the subtour
template<class T> void Little<T>::removeSubTour(Matrix<T> &m, int index, pair<int, int> &path) {
    int size = m.getNbRows();
    pair<int, int> pos;
    int founds = 0;
    vector<pair<int, int> > paths;

    // Research of all the included path
    while (index != 0) { // Iterate until we are not arrived at the root
        if (tree[index].bar == false) {
            paths.push_back(tree[index].path);
        }
        index = tree[index].parentNodeKey;
    }

    // Research of the longest subtour
    deque<int> subtour = {path.first, path.second};
    bool found = true;
    while (found) {
        found = false;
        for (const pair<int, int> &segment : paths) {
            // Check that "segment" go ahead in a subtour
            if (segment.second == subtour.front()) {
                subtour.push_front(segment.second);
                subtour.push_front(segment.first);
                found = true;
                break;
            }
            // Check that "segment" go behind in a subtour
            else if (segment.first == subtour.back()) {
                subtour.push_back(segment.first);
                subtour.push_back(segment.second);
                found = true;
                break;
            }
        }
    }

    // Research of the segment to delete in the matrix
    for (int i = 1; i < size; i++) {
        if (m.getValue(i, 0) == subtour.back()) {
            pos.first = i;
            founds++;
        }
        if (m.getValue(0, i) == subtour.front()) {
            pos.second = i;
            founds++;
        }
    }

    // If the segment to delete has been found, then delete it by giving him an infinite cost
    if (founds == 2) {
        m.setValue(pos.first, pos.second, this->infinity);
    }
}

// Add the city's number in the matrix
template<class T> void Little<T>::addIndices(Matrix<T> &m) {
    m.addRow(0);
    m.addColumn(0);
    int size = m.getNbRows();

    for (int i = 0; i < size; i++) {
        m.setValue(0, i, i);
        m.setValue(i, 0, i);
    }
}

/*
 * Order the path contained in a tree branch ending by the index id,
 * the ordered path will have the "begin" point has starting point
 */
template<class T> vector<int> Little<T>::orderPath(int index, int begin) {
    vector<pair<int, int> > path;
    vector<int> tour;

    // Retrieval of the path stored in a branch's tree
    while (index != 0) {    // Iterate until we are not arrived at the root
        if (tree[index].bar == false) {     // If it is a node without regret cost
            path.push_back(tree[index].path);   // then we add this segment to the path
        }
        index = tree[index].parentNodeKey;
    }

    // Research of the path segment containing begin
    int pathSize = path.size();
    for (int i = 0; i < pathSize; i++) {
        if (path[i].first == begin) {
            tour.push_back(path[i].first);
            tour.push_back(path[i].second);
            path.erase(path.begin() + i);
        }
    }

    // Ordering of the rest of the tour
    while (tour.size() != pathSize) {
        for (int i = 0; i < path.size(); i++) {
            if (tour.back() == path[i].first) {
                tour.push_back(path[i].second);
                path.erase(path.begin() + i);
            }
        }
    }

    return tour;
}

// Little algorithm
template<class T> void Little<T>::findTour() {
    Node<T> normalNode;     // node with regret
    Node<T> regretNode;     // node without regret
    regretNode.bar = true;
    pair<int, int> pos;     // var to store the position of a cell in the matrix
    stack<pair<int, Matrix<T> > > matrices;    // stack containing the necessary matrix to pursue other branch of the tree
    pair<int, Matrix<T> > matrix;   // matrix associated to a node

    // Init of the stack with the initial distances matrix
    matrix.first = 0;
    matrix.second = initialMatrix;
    matrices.push(matrix);

    while (!matrices.empty()) {     // Iterate till the stack is empty
        int id = matrices.top().first;
        Matrix<T> m = matrices.top().second;
        matrices.pop();

        // Reduction of the matrix and computation of the minimum sum (raw + col)
        normalNode.cost = reduceMatrix(m);
        if (id == 0) {      // root tree case
            tree.push_back(normalNode);
        }

        /* Until it ends up with a 2x2 matrix (3x3 du to the indexes storage)
         * and until the current node is lower than the reference value */
        while (m.getNbRows() > 3 and tree[id].cost < this->reference) {

#ifdef DEBUG
            if ((tree.size() - 1) % 10000 == 0) {
                cout << "\r" << tree.size() - 1 << " nodes ..." << std::flush;
            }
#endif

            // Compute the node with regret
            regretNode.cost = tree[id].cost + calculateRegret(m, normalNode.path, pos);
            regretNode.parentNodeKey = id;
            tree.push_back(regretNode);

            // Storing of the matrix
            if (regretNode.cost < this->reference) {
                matrix.first = tree.size() - 1;
                matrix.second = m;
                matrix.second.setValue(pos.first, pos.second,
                                       this->infinity);      // Suppression case i, j pour une potentielle recherche ulterieur
                matrices.push(matrix);
            }

            // Deletion raw col
            m.removeRow(pos.first);
            m.removeColumn(pos.second);

            // Subtour deletion
            removeSubTour(m, tree.size() - 1, normalNode.path);

            // Compute the node without regret
            normalNode.cost = tree[id].cost + reduceMatrix(m);
            normalNode.parentNodeKey = id;
            tree.push_back(normalNode);

            id = tree.size() - 1;
        }

        // Update of the best tour and the reference value
        if (m.getNbRows() == 3) {
            if (normalNode.cost < this->reference) {
                addLastPath(m);
                this->reference = normalNode.cost;
                this->lastTour = orderPath(tree.size() - 1, 1);

#ifdef DEBUG
                cout << "\r";
                checkTourCost();
                cout << "Cost " << this->reference;
                cout << " Tour ";
                for (int i = 0; i < this->lastTour.size(); i++) {
                    cout << this->lastTour[i] << " ";
                }
                cout << "Node " << tree.size() - 1;
                cout << endl;
#endif
            }
        }
    }

    this->optimal = true;   // Computing finished, the tour is thus optimal

#ifdef DEBUG
    cout << endl << tree.size() << " nodes visited" << endl;
#endif
}

// Add the two last segments of the tour when the matrix is 2x2
template<class T> void Little<T>::addLastPath(Matrix<T> &m) {
    Node<T> normalNode;

    for (int i = 1; i < 3; i++) {
        for (int j = 1; j < 3; j++) {
            if (m.getValue(i, j) == 0) {
                normalNode.path.first = m.getValue(i, 0);
                normalNode.path.second = m.getValue(0, j);
                normalNode.cost = tree.back().cost;
                normalNode.parentNodeKey = tree.size() - 1;
                tree.push_back(normalNode);
            }
        }
    }
}

template<class T> Little<T>::Little(Matrix<T> &m) {
#ifdef DEBUG
//    cout << m << endl;
#endif

    this->infinity = m.getEmptyValue();     // Retrieval of the emptyValue, that we consider as infinity
    addIndices(m);      // Addition of the indexes in the matrix
    this->initialMatrix = m;    // storage of the initial matrix
}

#ifdef DEBUG
/*
 * Calculate the cost of the last found tour.
 * Useful to verify that the cost stored in the nodes is correct
 */
template<class T> void Little<T>::checkTourCost() {
    int cost = 0;
    int size = this->lastTour.size();
    for (int i = 0; i < size - 1; i++) {
        cost += this->initialMatrix.getValue(this->lastTour[i], this->lastTour[i + 1]);
    }
    cost += this->initialMatrix.getValue(this->lastTour.back(), this->lastTour.front());
    cout << "Cost check " << cost << " ";
}
#endif

#endif  /* LITTLE_H */