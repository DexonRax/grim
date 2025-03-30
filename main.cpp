#include "grim.h"

int main(int argc, char **argv){
    std::string filename;
    if (argc > 1) {
        filename = argv[1];  // Use the first argument as the file to open.
    }else{
        filename = "";
    }
    Grim grim(filename);
    grim.run();
    return 0;
}
