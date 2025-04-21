#include <cassert>
#include <gmpxx.h>
#include "polygon.cpp"
#include "customFloat.cpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

TEST_CASE("LL Polygon constructor Number"){

    using Num = Number<int, 2, 5, 13, 17>;
    using Poly = Polygon::LLPoly<Num>;
    Poly poly{{}};
    REQUIRE(poly.get_head() == nullptr);
}


TEST_CASE("LL Polygon copy Number"){

    using Num = Number<int, 2, 5, 13, 17>;
    using Poly = Polygon::LLPoly<Num>;
    
    Poly poly1({{0,0}, {0,12}, {12,12}});
    Poly poly2(poly1);

    REQUIRE(poly1 == poly2);
}


TEST_CASE("LL Polygon flip_x Number"){

    using Num = Number<int, 2, 5, 13, 17>;
    using Poly = Polygon::LLPoly<Num>;
    
    Poly poly1({{0,0}, {0,12}, {12,12}});
    Poly poly2({{0,0}, {-12,12}, {0,12}});
    poly1.flip_x();

    REQUIRE(poly1 == poly2);
}




TEST_CASE("LL Polygon is_point_inside [Number]"){

    using Num = Number<int, 2, 5, 13, 17>;
    using Poly = Polygon::LLPoly<Num>;
    
    Poly poly({{0,0}, {12,0}, {12,12}, {6,6}, {0,12}}); // Anti-clockwise polygon
    Poly poly2({{0,12}, {6,6}, {12,12}, {12,0}, {0,0}}); // Clockwise polygon

    REQUIRE(poly.is_point_inside({10,2 }) == true);  // inside
    REQUIRE(poly.is_point_inside({12,10}) == false); // on the border
    REQUIRE(poly.is_point_inside({13,10}) == false); // outside

    REQUIRE(poly2.is_point_inside({10,2 }) == false);  // inside
    REQUIRE(poly2.is_point_inside({12,10}) == false); // on the border
    REQUIRE(poly2.is_point_inside({13,10}) == true); // outside

    REQUIRE(poly.is_point_inside({1,1}, {-1,-1}) == true);  // inside, cross vertex
    REQUIRE(poly.is_point_inside({5,5}, {13,13}) == true);  // inside, coincident edge
    REQUIRE(poly.is_point_inside({-1,0}, {13,0}) == false); // outside, coincident edge
}


TEST_CASE("LL Polygon intersection [Number]"){

    using Num = Number<int, 2, 5, 13, 17>;
    using Poly = Polygon::LLPoly<Num>;

    Poly poly1({{0,0}, {3,0}, {3,3}, {0,3}});
    Poly poly2({{1,1}, {2,1}, {2,2}, {1,2}});
    Poly poly3({{2,2}, {4,2}, {4,4}, {2,4}});
    Poly poly4({{1,0}, {2,0}, {2,1}, {1,1}});
    Poly poly5({{3,0}, {6,3}, {3,6}, {0,3}});
    Poly poly6({{0,0}, {0,1}, {1,1}, {2,2}, {2,0}, {1,-1}});
    Poly poly7({{0,0}, {1,1}, {2,0}});
    Poly poly8({{1,0}, {3,1}, {2,3}, {0,2}});
    Poly poly9({{1,0}, {2,0}, {2,3}, {1,3}});
    Poly poly10({{5,0}, {6,0}, {6,1}, {5,1}});


    REQUIRE(poly1.points_inside(poly2) == true);
    REQUIRE(poly2.points_inside(poly1) == false);
    REQUIRE(poly1.points_inside(poly3) == true);
    REQUIRE(poly3.points_inside(poly1) == true);

    REQUIRE(poly1.points_inside(poly1) == false);
    REQUIRE(poly4.points_inside(poly3) == false);

    REQUIRE(poly1.edge_edge_intersection(poly2) == false);
    REQUIRE(poly1.edge_edge_intersection(poly3) == true);
    REQUIRE(poly1.edge_edge_intersection(poly4) == false);

    REQUIRE(poly1.node_node_intersection(poly5) == true);
    REQUIRE(poly1.node_node_intersection(poly4) == false);
    REQUIRE(poly6.node_node_intersection(poly7) == true);

    REQUIRE(poly1.edge_node_intersection(poly8) == true);
    REQUIRE(poly8.edge_node_intersection(poly1) == false);
    REQUIRE(poly1.edge_node_intersection(poly9) == true);
    REQUIRE(poly9.edge_node_intersection(poly1) == false);

    REQUIRE(poly1.overlaps(poly2));
    REQUIRE(poly1.overlaps(poly3));
    REQUIRE(poly1.overlaps(poly1));
    REQUIRE(poly1.overlaps(poly5));
    REQUIRE(poly6.overlaps(poly7));
    REQUIRE(poly1.overlaps(poly8));
    REQUIRE(poly1.overlaps(poly9));
    REQUIRE(!poly1.overlaps(poly10));
}



TEST_CASE("LL Polygon merge [Number]"){

    using Num = Number<int, 2, 5, 13, 17>;
    using Poly = Polygon::LLPoly<Num>;

    Poly poly1({{0,0}, {0,12}, {12,12}});
    Poly poly2({{0,0}, {0,6}, {6,6}});
    Poly poly3({{0,0}, {0,6}, {6,6}, {0,0}, {0,12}, {12,12}});

    poly2.merge(poly2.get_head(), poly1, poly1.get_head());
    REQUIRE(poly2 == poly3);
}


TEST_CASE("LL Polygon prune [Number]"){

    using Num = Number<int, 2, 5, 13, 17>;
    using Poly = Polygon::LLPoly<Num>;
    using V = Vertex<Num>;

    Poly poly1({{0,0}, {0,12}, {12,12}});
    Poly poly2({{0,0}, {0,6}, {6,6}});

    poly2.merge(poly2.get_head(), poly1, poly1.get_head());

    V *node = poly2.get_head();
    for(unsigned i=0; i<poly2.size()*2-1; i++){
        node = node->next;
    }
    REQUIRE(node == poly2.get_head()->prev);

    node = poly2.get_head();
    for(unsigned i=0; i<poly2.size()*2+1; i++){
        node = node->prev;
    }
    REQUIRE(node == poly2.get_head()->prev);

}


TEST_CASE("LL Polygon prune square+triangle [Number]"){

    using Num = Number<int, 2, 5, 13, 17>;
    using Poly = Polygon::LLPoly<Num>;
    using V = Vertex<Num>;

    Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
    Poly poly2({{0,0}, {3,0}, {3,3}, {0,3}});
    Poly poly3({{3,0}, {3,3}, {0,3}, {0,12}, {12,12}, {12,0}});

    V *node1 = poly1.get_head();
    V *node2 = poly2.get_head();
    auto getter = []([[maybe_unused]] unsigned int i){return 0;};
    poly2.merge(node2, poly1, node1);
    poly2.prune_LL({node1, node2}, getter);
    REQUIRE(poly2 == poly3);

}


TEST_CASE("LL Polygon prune two rectangles [Number]"){

    using Num = Number<int, 2, 5, 13, 17>;
    using Poly = Polygon::LLPoly<Num>;
    using V = Vertex<Num>;

    Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
    Poly poly2({{0,0}, {12,0}, {12,3}, {0,3}});
    Poly poly3({{12,3}, {0,3}, {0,12}, {12,12}});
    
    V *temp1 = poly1.get_head();
    V *temp2 = poly2.get_head();
    auto getter = []([[maybe_unused]] unsigned int i){return 0;};
    poly2.merge(poly2.get_head(), poly1, poly1.get_head());
    poly2.prune_LL({temp1, temp2}, getter);

    REQUIRE(poly2 == poly3);
}

TEST_CASE("LL Polygon prune two identical rectangles [Number]"){

    using Num = Number<int, 2, 5, 13, 17>;
    using Poly = Polygon::LLPoly<Num>;
    using V = Vertex<Num>;

    Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
    Poly poly2({{0,0}, {12,0}, {12,12}, {0,12}});

    V *node1 = poly1.get_head();
    V *node2 = poly2.get_head();
    auto getter = [](unsigned int i){return 0;};
    poly2.merge(node2, poly1, node1);
    poly2.prune_LL({node1, node2}, getter);
    REQUIRE(poly2.get_head() == nullptr);  

}


TEST_CASE("LL Polygon translation and rotation [Number]"){
    using Num = Number<int, 2, 5, 13, 17>;
    using Poly = Polygon::LLPoly<Num>;
    using V = Vertex<Num>;

    Poly poly1({{0,0}, {0,12}, {12,12}, {12,0}});
    Poly poly2({{8,0}, {12,0}, {12,3}});
    V *R1 = poly1.get_head()->next;
    V *R2 = poly2.get_head();
    poly2.translate(R1->position-R2->position);
    REQUIRE(poly1.overlaps(poly2) == true);        
    poly2.rotate(R1->angle_end - R2->angle_start, R1->position);
    REQUIRE(poly1.overlaps(poly2) == false);
}


TEST_CASE("LL Polygon translation and rotation [gmp]"){

    using Num = Number<mpz_class, 2, 5, 13, 17>;
    using Poly = Polygon::LLPoly<Num>;
    using V = Vertex<Num>;

    Polygon::LLPoly<Num> poly1({{0,0}, {0,12}, {12,12}, {12,0}});
    Polygon::LLPoly<Num> poly2({{8,0}, {12,0}, {12,3}});
    Vertex<Num> *R1 = poly1.get_head()->next;
    Vertex<Num> *R2 = poly2.get_head();
    poly2.translate(R1->position-R2->position);
    REQUIRE(poly1.overlaps(poly2) == true);        
    poly2.rotate(R1->angle_end - R2->angle_start, R1->position);
    REQUIRE(poly1.overlaps(poly2) == false);

}


TEST_CASE("LL Polygon get_obtusest [Number]"){

    using Num = Number<mpz_class, 2, 5, 13, 17>;
    using Poly = Polygon::LLPoly<Num>;
    using V = Vertex<Num>;


    Poly poly1({{0,0}, {0,12}, {12,12}});
    Poly poly2({{0,0}, {-12,12}, {0,12}});
    Poly poly3({{0,0}, {0,12}, {3,0}});

    unsigned index1 = poly1.get_obtusest_node();
    unsigned index2 = poly2.get_obtusest_node();
    unsigned index3 = poly3.get_obtusest_node();

    const V *node1 = poly1.vertex_from_index(index1);
    const V *node2 = poly2.vertex_from_index(index2);
    const V *node3 = poly3.vertex_from_index(index3);
    
    REQUIRE(node1 == poly1.get_head());
    REQUIRE(node2 == poly2.get_head());
    REQUIRE(node3 == poly3.get_head()->next);
}