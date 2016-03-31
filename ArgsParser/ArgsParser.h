#ifndef ARGSPARSER_H
#define	ARGSPARSER_H

#include <string>

using std::string;

class ArgsParser {
private:
    int argc;
    char** argv;
    
public:
    ArgsParser(int argc, char** argv) : argc(argc), argv(argv) {}
    void exec();
    string getParam(string);
};


#endif	/* ARGSPARSER_H */