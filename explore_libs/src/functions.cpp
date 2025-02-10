#pragma once
#include "functions.hpp"
#include <iostream>

template <typename T>
void MyClass<T>::talker(){
    std::cout << "Talk!" << std::endl;    
}