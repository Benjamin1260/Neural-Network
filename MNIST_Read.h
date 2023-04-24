#include <iostream>
#include <string>
#include <fstream>

std::string test() {
    return "Succes!";
};

//initialize config by using first 4 bytes (magic number)
//each line = one picture (28*28)
//custArr<custArr<custArr<int>>> ==> link correct value to image: pair?
//return