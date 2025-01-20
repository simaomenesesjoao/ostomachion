#include "polygon.cpp"
#include <cassert>


int main(){

    using Poly = Polygon<2,5,13,17>;
    using Nod = Node<2,5,13, 17>;

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

    return 0;
}