#ifndef POLYGON_C
#define POLYGON_C

#include <list>
#include "points_vectors.cpp"

template <int... Ints>
class Node{
    using Angle = Angle<Ints...>;
    using Point = Point<Ints...>;

public:
    Node(Point const& P): position{P}{};

    void update_opening(){
        if(end_init and start_init)
            angle_opening = angle_end - angle_start;
        
    }

    void update_end(Angle const& A){
        angle_end = A;
        update_opening();
    }

    void update_start(Angle const& A){
        angle_end = A;
        update_opening();
    }

private:
    Point position;
    Angle angle_start, angle_end, angle_opening;
    bool end_init, start_init;
};

template <int... Ints>
class Polygon{
    using Node = Node<Ints...>;
    using Point = Point<Ints... >;

private:
    std::list<Node> nodes;
    
    void append(Point const& point){
        if(nodes.size() == 0){
            nodes.push_back(Node(point));
        } else {
            nodes.push_back(Node(point));
            auto last = nodes.end();
            auto prev = last;
            prev--;
            Angle angle{*last - *prev};
            prev->update_start(angle);
            last->update_end(-angle);
        }
    };


public:
    template <typename... T>
    Polygon(T... points){

    }

};


#endif // POLYGON_C