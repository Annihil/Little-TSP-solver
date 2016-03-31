#include "ArgsParser.h"
#include "../TSPLIB/tsplib.h"

using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

void ArgsParser::exec() {
    string inputParam = getParam("-i");
    string outputParam = getParam("-o");
    if (inputParam == "") {
        cout << "Error : -i parameter not detected" << endl;
        return;
    }

    ifstream inputFile(inputParam);
    if (!inputFile) {
        cout << "Error : Input file not found" << endl;
        return;
    }

    if (outputParam != "") {
        ofstream outputFile(outputParam);
        if (!outputFile) {
            cout << "Error : Output file cannot be written" << endl;
            inputFile.close();
            return;
        }

        // -i and -o options
        Tsplib tsp(inputFile, outputFile);
        outputFile.close();
    }
    else {
        // -i but no -o option
        Tsplib tsp(inputFile);
    }
    inputFile.close();
}

string ArgsParser::getParam(string cmd) {
    for (int i = 0; i < argc; i++) {
        if (argv[i] == cmd && i < argc - 1) {
            return argv[i + 1];
        }
    }
    return "";
}