#include "intersection_algorithms.cpp"
#include <cassert>
#include "polygon.cpp"

int main(){
    {
        std::cout << "Testing_edges_intersect" << std::endl;
        using P = Point<2,3>;
        P A{0,0}, B{1,2}, C{0,1}, D{1,0}, E{1,-1};

        //   | B
        //   C
        // --AD---->
        //   |E

        assert(edges_intersect(A, B, C, D) == true);
        assert(edges_intersect(A, E, C, D) == false);
        assert(edges_intersect(A, B, A, B) == false);
        assert(edges_intersect(A, B, D, E) == false);
        assert(edges_intersect(A, C, A, D) == false);
        assert(edges_intersect(C,-C, A, D) == false);
    }


      {
        std::cout << "Testing point_on_edge" << std::endl;
        using P = Point<2,3>;
        P A{0,0}, B{1,1}, C{2,2}, D{1,0}, E{1,-1};

        //   | C
        //   |B
        // --AD---->
        //   |E

        assert(point_on_edge(A, B, C) == false);
        assert(point_on_edge(A, C, B) == true);
        assert(point_on_edge(A, B, D) == false);
        assert(point_on_edge(A, B, B) == false);
        
    }


    {
        std::cout << "Testing edge_intersects_vertex" << std::endl;

        Polygon<2,3> poly({{0,0}, {5,0}, {5,5}, {0,5}});

        assert(is_inner_vertex({-1,-1}, {1,1}, poly.head) == false);
        assert(is_inner_vertex({-1,1}, {1,-1}, poly.head) == false);
        assert(is_inner_vertex({1,-1}, {-1,1}, poly.head) == true);
        
        
    }


    {
        std::cout << "Testing edge_splits_vertex" << std::endl;

        Polygon<2,3> poly({{0,0}, {5,0}, {5,5}, {0,5}});

        assert(edge_splits_vertex({-1,-1}, {1,1}, poly.head) == true);
        assert(edge_splits_vertex({-1,1}, {1,-1}, poly.head) == false);
        assert(edge_splits_vertex({1,-1}, {-1,1}, poly.head) == false);
        
    }



    {
        std::cout << "Testing shoelace area formula" << std::endl;
        Point<2,3> A{0,0}, B{1,0}, C{1,1}, D{0,1}; 
        assert((shoelace_area<2,3>({A,B,C,D})) == 1); // anticlockwise polygon > 0
        assert((shoelace_area<2,3>({D,C,B,A})) == -1); // clockwise polygon < 0

    }



    {
        std::cout << "Testing nodes_compatible" << std::endl;

        Node<2,3,5> M({0,0}), N({0,0}), P({0,0}), A({0,0}), B({0,0});


        // /_
        A.update_start({1,1});
        A.update_end({1,0});


        // //
        B.update_start({1,2});
        B.update_end({2,1});

        // /_
        M.update_start({1,0});
        M.update_end({1,1});

        // _|
        N.update_start({0,1});
        N.update_end({-1,0});

        // |_
        P.update_start({1,0});
        P.update_end({0,1});

        assert(nodes_compatible(M,N) == true);
        assert(nodes_compatible(M,P) == false);
        assert(nodes_compatible(N,P) == true);

        assert(nodes_compatible(A,M) == true);
        assert(nodes_compatible(A,P) == false);
        assert(nodes_compatible(B,P) == false);
    }

}