#include "polygon.cpp"

int main(){

    Polygon poly1;
    poly1.append(Point{0,0});
    poly1.append(Point{3,0});
    poly1.append(Point{2,4});

    std::cout << poly1 << "\n";
    return 0;
}