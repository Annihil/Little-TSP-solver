#ifndef INDEXOUTOFBOUNDSEXCEPTION_H
#define	INDEXOUTOFBOUNDSEXCEPTION_H

#include <iostream>
#include <sstream>
#include <exception>

class IndexOutOfBoundsException : public std::exception {
private:
    std::string msg;
    
public:
    IndexOutOfBoundsException(int index, int lowerBound, int upperBound) {
        std::ostringstream oss;
        oss << "IndexOutOfBoundsException : the index " << index << " violates the bounds [" << lowerBound << " - " << upperBound << "]";
        this->msg = oss.str();
    };
   
    virtual const char * what() const throw() {
        return this->msg.c_str();
    };
};

#endif	/* INDEXOUTOFBOUNDSEXCEPTION_H */