#include "polygon.cpp"
#include <cassert>


int main(){

    using Poly = Polygon<2,5,13,17>;
    using Nod = Node<2,5,13, 17>;
    // using Poin = Point<2,5,13,17>;



    {
        std::cout << "Testing polygon constructor" << std::endl;
        Poly poly{{}};
        assert(poly.head == nullptr);
        
    }

    {
        std::cout << "Testing polygon copy" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}});
        Poly poly2(poly1);
   
        assert(poly2 == poly1);
    }

    {
        std::cout << "Testing polygon flip_x" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}});
        Poly poly2({{0,0}, {-12,12}, {0,12}});
        poly1.flip_x();
        assert(poly2 == poly1);
    }



    // {
    //     std::cout << "Testing is_point_inside" << std::endl;
    //     // Anti-clockwise polygon
    //     Poly poly({{0,0}, {12,0}, {12,12}, {6,6}, {0,12}});

    //     assert(poly.is_point_inside({10,2 }) == true);  // inside
    //     assert(poly.is_point_inside({12,10}) == false); // on the border
    //     assert(poly.is_point_inside({13,10}) == false); // outside

    //     assert(poly.is_point_inside({1,1}, {-1,-1}) == true);  // inside, cross vertex
    //     assert(poly.is_point_inside({5,5}, {13,13}) == true);  // inside, coincident edge
    //     assert(poly.is_point_inside({-1,0}, {13,0}) == false); // outside, coincident edge
        

    // }
    

    {
        std::cout << "Testing polygon intersection" << std::endl;

        Poly poly1({{0,0}, {3,0}, {3,3}, {0,3}});
        Poly poly2({{1,1}, {2,1}, {2,2}, {1,2}});
        Poly poly3({{2,2}, {4,2}, {4,4}, {2,4}});
        Poly poly4({{1,0}, {2,0}, {2,1}, {1,1}});
        Poly poly5({{3,0}, {6,3}, {3,6}, {0,3}});
        Poly poly6({{0,0}, {0,1}, {1,1}, {2,2}, {2,0}, {1,-1}});
        Poly poly7({{0,0}, {1,1}, {2,0}});
        Poly poly8({{1,0}, {3,1}, {2,3}, {0,2}});
        Poly poly9({{1,0}, {2,0}, {2,3}, {1,3}});
        Poly poly10({{0,0}, {4,0}, {2,3}, {1,2}});


        assert(poly1.points_inside(poly2) == true);
        assert(poly2.points_inside(poly1) == false);
        assert(poly1.points_inside(poly3) == true);
        assert(poly3.points_inside(poly1) == true);

        assert(poly1.points_inside(poly1) == false);
        assert(poly4.points_inside(poly3) == false);

        assert(poly1.edge_edge_intersection(poly2) == false);
        assert(poly1.edge_edge_intersection(poly3) == true);
        assert(poly1.edge_edge_intersection(poly4) == false);

        assert(poly1.node_node_intersection(poly5) == true);
        assert(poly1.node_node_intersection(poly4) == false);
        assert(poly6.node_node_intersection(poly7) == true);

        assert(poly1.edge_node_intersection(poly8) == true);
        assert(poly8.edge_node_intersection(poly1) == false);
        assert(poly1.edge_node_intersection(poly9) == true);
        assert(poly9.edge_node_intersection(poly1) == false);

        poly1.overlaps(poly2);
        poly1.overlaps(poly3);
        poly1.overlaps(poly1);
        poly1.overlaps(poly5);
        poly6.overlaps(poly7);
        poly1.overlaps(poly8);
        poly1.overlaps(poly9);
        poly1.overlaps(poly10);



    }










    {
        std::cout << "Testing simple merge" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}});
        Poly poly2({{0,0}, {0,6}, {6,6}});
        Poly poly3({{0,0}, {0,6}, {6,6}, {0,0}, {0,12}, {12,12}});
   
        poly2.merge(poly2.head, poly1, poly1.head);
        assert(poly2 == poly3);
    }

    {
        std::cout << "Testing linked list after merge" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}});
        Poly poly2({{0,0}, {0,6}, {6,6}});

        poly2.merge(poly2.head, poly1, poly1.head);
        
        LL_Node<Nod> *node = poly2.head;
        for(unsigned i=0; i<poly2.size_ll*2-1; i++){
            node = node->next;
        }
        assert(node == poly2.head->prev);

        node = poly2.head;
        for(unsigned i=0; i<poly2.size_ll*2+1; i++){
            node = node->prev;
        }
        assert(node == poly2.head->prev);
    }

    {
        std::cout << "Testing pruning square+triangle" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
        Poly poly2({{0,0}, {3,0}, {3,3}, {0,3}});
        Poly poly3({{3,0}, {3,3}, {0,3}, {0,12}, {12,12}, {12,0}});
   
        LL_Node<Nod> *node1 = poly1.head;
        LL_Node<Nod> *node2 = poly2.head;
        poly2.merge(node2, poly1, node1);
        poly2.prune_LL({node1, node2});
        assert(poly2 == poly3);
    }

    {
        std::cout << "Testing pruning two rectangles" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
        Poly poly2({{0,0}, {12,0}, {12,3}, {0,3}});
        Poly poly3({{12,3}, {0,3}, {0,12}, {12,12}});
   
        LL_Node<Nod> *temp1 = poly1.head;
        LL_Node<Nod> *temp2 = poly2.head;
        poly2.merge(poly2.head, poly1, poly1.head);
        poly2.prune_LL({temp1, temp2});

        assert(poly2 == poly3);
    }

    {
        std::cout << "Testing pruning two identical rectangles" << std::endl;
        Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
        Poly poly2({{0,0}, {12,0}, {12,12}, {0,12}});

        LL_Node<Nod> *node1 = poly1.head;
        LL_Node<Nod> *node2 = poly2.head;
        poly2.merge(node2, poly1, node1);
        poly2.prune_LL({node1, node2});
        assert(poly2.head == nullptr);  
    }

}