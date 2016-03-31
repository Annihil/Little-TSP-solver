#include "ArgsParser/ArgsParser.h"
#include "Little/Little.h"
#include <ctime>

using std::cout;
using std::endl;

int main(int argc, char** argv) {
    #ifdef DEBUG
    std::clock_t start = std::clock();
    #endif

    ArgsParser argsParser(argc, argv);
    argsParser.exec();
    
    #ifdef DEBUG
    double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << "Total Running Time : " << duration << " (seconds)" << endl;
    #endif
    
    return 0;
}