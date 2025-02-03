#pragma once

// #include "points_vectors.hpp"
#include "points_vectors.cpp"
#include "shoelace.hpp"
#include <utility>
#include <cassert>

template <typename Num>
bool edges_intersect(Point<Num> const& P1, Point<Num> const& P2, 
                     Point<Num> const& Q1, Point<Num> const& Q2){
    using V = Point<Num>;
    
    V q{Q2-Q1}, p{P2-P1}, v{P1-Q1};
    // std::cout << "arguments for edges_intersect: " << std::endl;
    // std::cout << P1 << std::endl;
    // std::cout << P2 << std::endl;
    // std::cout << Q1 << std::endl;
    // std::cout << Q2 << std::endl;

    // Check if the edges are parallel
    if(q.get_x()*p.get_y() == q.get_y()*p.get_x()){
        // std::cout << "parallel" << std::endl;
        return false;
    }
    
    // Find vectors perpendicular to each of the edges
    V perp_q{q.get_y(), -q.get_x()};
    V perp_p{p.get_y(), -p.get_x()};

    Num pdv = perp_p.dot(v);
    Num qdv = perp_q.dot(v);

    // q t - p s = v , s and t between 0 and 1
    // - perp_q . p s = perp_q . v
    // perp_p . q t = perp_p . v

    // std::cout << "perp_p.dot(v): " << perp_p.dot(v) << std::endl;
    // std::cout << "perp_q.dot(v): " << perp_q.dot(v) << std::endl;
    // std::cout << "perp_q.dot(p): " << perp_q.dot(p) << std::endl;
    // std::cout << "perp_p.dot(q): " << perp_p.dot(q) << std::endl;

    // Num s = -perp_q.dot(v)/perp_q.dot(p);
    // Num t =  perp_p.dot(v)/perp_p.dot(q);

    Num s_num = -perp_q.dot(v);
    Num s_den =  perp_q.dot(p);
    Num t_num =  perp_p.dot(v);
    Num t_den =  perp_p.dot(q);

    if(s_den<0){
        s_num = -s_num;
        s_den = -s_den;
    } 

    if(t_den<0){
        t_num = -t_num;
        t_den = -t_den;
    }

    // std::cout << (float)s_num << " " << (float)s_den << " ";
    // std::cout << (float)t_num << " " << (float)t_den << std::endl;

    // std::cout << "s,t: " << s_num << " " << s_den << " " << t_num << " " << t_den << std::endl;

    return s_num > 0 and s_num < s_den and t_num > 0 and t_num < t_den;

}



template <typename Num>
bool point_on_edge(Point<Num> const& P, Point<Num> const& Q, 
                    Point<Num> const& V){
    // Check if the point V lies exactly on the line segment connecting P->Q

    Point<Num> q{Q-P};
    Num dq = q.dot(V-P);
    

    if(dq <= 0 or dq >= q.dot(q))
        return false;

    return q.cross(V-P)==0;
}



template <typename Num>
bool edge1_includes_edge2(Point<Num> const& A, Point<Num> const& B, 
                    Point<Num> const& C, Point<Num> const& D){
    return point_on_edge(A, B, C) and point_on_edge(A, B, D);
}


template <typename Num>
bool is_inner_vertex(Point<Num> const& P, Point<Num> const& Q, 
                            LL_Node<Node<Num>> *pointer_V){
    // Check whether the edges connecting V open to the outside
    // of the polygon which contains P and Q

    assert(pointer_V != nullptr);
    assert(pointer_V->next != nullptr and pointer_V->prev != nullptr);

    Point<Num> const& V = pointer_V->data.position;
    Point<Num> const& Vn = pointer_V->next->data.position;
    Point<Num> const& Vp = pointer_V->prev->data.position;
    Point<Num> q{Q-P};

    // If V isn't between P and Q, this algorithm doesn't make sense
    assert(point_on_edge(P, Q, V));
    

    // std::cout << "current, prev, next" << std::endl;
    // std::cout << "V: "  << V  << std::endl;
    // std::cout << "Vp: " << Vp << std::endl;
    // std::cout << "Vn: " << Vn << std::endl;

    // std::cout << "cross next: " << q.cross(Vn-V) << std::endl;
    // std::cout << "cross prev: " << q.cross(Vp-V) << std::endl << std::endl;
    
    return q.cross(Vn-V) <= 0 and q.cross(Vp-V) <= 0;

}


template <typename Num>
bool edge_splits_vertex(Point<Num> const& P, Point<Num> const& Q, 
                            LL_Node<Node<Num>> *pointer_V){
    // Checks whether an edge goes right through a vertex in such a way 
    // that the two edges coming out of that vertex are in opposite 
    // sides of this edge
    // This function is very similar to is_inner_vertex, but 
    // the final comparison is different

    assert(pointer_V != nullptr);
    assert(pointer_V->next != nullptr and pointer_V->prev != nullptr);

    Point<Num> const& V = pointer_V->data.position;
    Point<Num> const& Vn = pointer_V->next->data.position;
    Point<Num> const& Vp = pointer_V->prev->data.position;
    Point<Num> q{Q-P};

    // If V isn't between P and Q, this algorithm doesn't make sense
    assert(point_on_edge(P, Q, V));
    

    // std::cout << "current, prev, next" << std::endl;
    // std::cout << "V: "  << V  << std::endl;
    // std::cout << "Vp: " << Vp << std::endl;
    // std::cout << "Vn: " << Vn << std::endl;

    // std::cout << "cross next: " << q.cross(Vn-V) << std::endl;
    // std::cout << "cross prev: " << q.cross(Vp-V) << std::endl << std::endl;
    Num x1 = q.cross(Vn-V);
    Num x2 = q.cross(Vp-V);

    if(x1 == 0 or x2 == 0)
        return false;
    
    return (x1 < 0) != (x2 < 0);
}


template <typename Num>
bool coincident_edges_diverge(Point<Num> const& P, Point<Num> const& Q, 
                    LL_Node<Node<Num>> *A, LL_Node<Node<Num>> *B){
    // Check if A and B open to different sides. If they do, then the line P-Q
    // is traversing the polygon from in to out
    //     |                \       /     
    //     |                 \     /      
    // P---A===B---Q     P---A===B---Q  
    //        /
    //       /

    assert(edge1_includes_edge2(P, Q, A->data.position, B->data.position));
    assert(A->next == B or B->next == A);

    // The previous lines ensure that A and B lie on the same line as PQ, and that they are neighbours.
    // By construction, there cannot be angles of 0 or 180º, so when calling edge_intersects_vertex, 
    // this is just checking to which direction it's opening. True = left, False = right
    return !is_inner_vertex(P, Q, A) != !is_inner_vertex(P, Q, B);
    // return false;
}


template <typename Num>
bool nodes_compatible(Node<Num> const& node1, Node<Num> const& node2){
    
    Point<Num> S1{node1.angle_start.get_cos(), node1.angle_start.get_sin()};
    Point<Num> E1{node1.angle_end.get_cos(),   node1.angle_end.get_sin()};
    Point<Num> S2{node2.angle_start.get_cos(), node2.angle_start.get_sin()};
    Point<Num> E2{node2.angle_end.get_cos(),   node2.angle_end.get_sin()};

    // std::cout << "S1" << S1 << std::endl;
    // std::cout << "E1" << E1 << std::endl;
    // std::cout << "S2" << S2 << std::endl;
    // std::cout << "E2" << E2 << std::endl;

    if(S1 == S2 and E1 == E2){
        // std::cout << "2 coincident" << std::endl;
        return false;
    }
        

    if(S1 == E2 and S2 == E1){
        // std::cout << "2 coincident" << std::endl;
        return true;
    }

    if(S1 == S2 or S1 == E2 or E1 == S2 or E1 == E2){
        // std::cout << "3 coincident" << std::endl;
        return shoelace_area<Num>({S1,E1,S2,E2}) > 0; // > 0 means polygon is anticlockwise
    }


    // Case 3: All points are distinct
    if(edges_intersect(S1, E2, S2, E1) or edges_intersect(S1, E1, S2, E2)){
        // std::cout << "node: edges intersect" << std::endl;
        return false;

    }
        
    // std::cout << "all edges are different" << std::endl;

    return shoelace_area<Num>({S1,E1,S2,E2}) > 0;
}


template <typename Num>
bool angles_compatible(Angle<Num> const& a, Angle<Num> const& b){
    bool a_larger_180 = a.is_larger_than_180();
    bool b_larger_180 = b.is_larger_than_180();
    if(a_larger_180 and b_larger_180)
        return false;

    if(not a_larger_180 and not b_larger_180)
        return true;

    Angle<Num> c{a+b};

    return c.is_larger_than_180() or c.is_zero();


}