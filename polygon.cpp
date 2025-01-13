#ifndef POLYGON_C
#define POLYGON_C

#include <list>
#include "points_vectors.cpp"
#include <iostream>

template <int... Ints>
class Node{
    using Ang = Angle<Ints...>;
    using Poin = Point<Ints...>;

public:
    Node(Poin const& P): position{P}{};

    void update_opening(){
        if(end_init and start_init)
            angle_opening = angle_end - angle_start;
        
    }

    void update_end(Ang const& A){
        angle_end = A;
        end_init = true;
        update_opening();
    }

    void update_start(Ang const& A){
        angle_start = A;
        start_init = true;
        update_opening();
    }
    Poin position;

    template <int...Args>
    friend std::ostream& operator<<(std::ostream& os, Node<Args...> const& node);

private:
    Ang angle_start, angle_end, angle_opening;
    bool end_init, start_init;
};

template <int... Ints>
class Polygon{

private:
    using Nod = Node<Ints...>;
    using Poin = Point<Ints... >;

public:
    std::list<Nod> nodes;
    
    void append(Poin const& point){
        if(nodes.size() == 0){
            nodes.push_back(Nod(point));
        } else {
            nodes.push_back(Nod(point));
            auto last = nodes.end();
            last--;
            auto prev = last;
            prev--;
            Angle angle{last->position - prev->position};
            prev->update_start(angle);
            last->update_end(-angle);
        }
    };


    template <typename... T>
    Polygon(T... points){

    }

};

template <int... Ints>
std::ostream& operator<<(std::ostream& stream, Polygon<Ints...> const& poly){
    for(auto& node: poly.nodes){
        stream << node << "\n";
    }
    return stream;
}

template <int... Ints>
std::ostream & operator<<(std::ostream& stream, Node<Ints...> const& node){
    std::cout << "( " << node.position << ", " 
                      << node.angle_start << ", "
                      << node.angle_end << ")";
    return stream;
}

#endif // POLYGON_C