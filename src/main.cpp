#include "cli.h"
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    CLI::handle(args);
    return 0;



    
}