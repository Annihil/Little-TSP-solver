#ifndef NEGATIVEDIMENSIONEXCEPTION_H
#define	NEGATIVEDIMENSIONEXCEPTION_H

// Standard input/output stream
#include <iostream>

// String buffer
#include <sstream>

// std::exception
#include <exception>

class NegativeDimensionException : public std::exception {
private:
    std::string msg;
    
public:
    NegativeDimensionException() {
        std::ostringstream oss;
        oss << "NegativeDimensionException : the indexes must be positives or null";
        this->msg = oss.str();
    };
    
    virtual ~NegativeDimensionException() throw()
    {
 
    }
   
    virtual const char * what() const throw() {
        return this->msg.c_str();
    };
};

#endif	/* NEGATIVEDIMENSIONEXCEPTION_H */