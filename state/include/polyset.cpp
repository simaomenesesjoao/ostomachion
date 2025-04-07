#pragma once

#include "polygon.cpp"
#include <functional>
#include <vector>

template <typename Poly, template <typename> class Group, template <typename> class Set>
class PolySet {
private:
    unsigned long long hash_;
    std::vector<std::vector<Poly>> polygons;
    unsigned int size_;

    bool equals(const PolySet<Poly, Group, Set>& other, std::function<Poly(const Poly&)> transform) const {
        // Compare the positions of each set of polygons. Returns true if all
        // of them are identical. Allows a transformation to be applied to the 
        // first set, so that two sets can be identical if they differ by 
        // rotation or reflection

        if(get_size() != other.get_size())
            return false;

        for(unsigned int i = 0; i < Set<Poly>::num_distinct_polys; i++){
            auto& type = polygons.at(i);
            auto& other_type = other.get_poly_at(i);

            if(type.size() != other_type.size())
                return false;

            for(auto& poly: type){
                bool found = false;
                auto transformed = transform(poly);
                for(auto& other_poly: other_type){
                    if(transformed == other_poly){
                        found = true;
                        break;
                    }
                }

                if(not found)
                    return false;
            }
        }
        return true;
    }

public:
    using G = Group<Poly>;
    using S = Set<Poly>;

    PolySet():polygons(S::num_distinct_polys), size_{0}{
        calculate_hash();
    };

    unsigned int get_size() const {
        return size_;
    }

    void calculate_hash(){
        // Calculates a hash that is invariant under rotation and reflection
        unsigned long long int h = 0;

        for(auto& f: Group<Poly>::transformations){
            for(auto& polyrow: this->polygons){
                for(auto& poly: polyrow){
                    unsigned long long h_ = 3991*f(poly).get_hash();
                    h += h_;
                    h = h%94619837451;
                }
            }
        }
        hash_ = h;
    }

    unsigned long long get_hash() const {
        return hash_;
    };

    const std::vector<Poly>& get_poly_at(unsigned int i) const {
        return polygons.at(i);
    }


    const std::vector<std::vector<Poly>>& get_all_polys() const {
        return polygons;
    }


    const Poly get_prepoly_at(unsigned int i) const {
        return Set<Poly>::pre_polys.at(i).first;
    }

    const Poly& get_irreducible_section() const {
        return Set<Poly>::irreducible_section;
    }

    bool is_type_available(unsigned int i) const {
        unsigned int a = polygons.at(i).size();
        unsigned int b = Set<Poly>::pre_polys.at(i).second;
        return a < b;
    }

    std::vector<unsigned int> find_available_types() const {
        // returns a vector with the indices of all the polygon types
        // that are not complete yet
        std::vector<unsigned int> available_types;
        for(unsigned i=0; i<Set<Poly>::num_distinct_polys; i++){
            if(polygons.at(i).size() < Set<Poly>::pre_polys.at(i).second)
                available_types.push_back(i);
        }
        return available_types;
    }

    static unsigned int get_max_size(){
        return Set<Poly>::max_size;
    }
    
    unsigned int get_num_distinct_polys() const {
        return Set<Poly>::num_distinct_polys;
    }
    
    void insert(unsigned int i, const Poly& poly) {
        auto& polyrow = this->polygons.at(i);
        assert(poly.size_ll == Set<Poly>::pre_polys.at(i).first.size_ll);
        assert(polyrow.size() < Set<Poly>::pre_polys.at(i).second);
        polyrow.push_back(poly);
        calculate_hash();
        size_++;
    }

    bool equals_under_symmetry(const PolySet& other, std::vector<std::function<Poly(const Poly&)>> transformations) const {

        for(auto& f: transformations){
            if(equals(other, f))
                return true;
        }
        return false;
    }

    bool strict_equality(const PolySet& other) const {
        return equals(other, [](const Poly& poly){return poly;});
    }

    bool operator==(const PolySet& other) const {
        return equals_under_symmetry(other, Group<Poly>::transformations);
    }



    std::vector<std::vector<std::vector<double>>> as_vector() const {
        std::vector<std::vector<std::vector<double>>> polygon_vector;
        for(const auto& polyrow: polygons)
            for(const auto& poly: polyrow)
                polygon_vector.push_back(poly.as_vector());
        return polygon_vector;
    }
};

template <typename Poly, template <typename> class Group, template <typename> class Set>
std::ostream& operator<<(std::ostream& stream, const PolySet<Poly, Group, Set>& polyset){
    for(unsigned int i=0; i<polyset.get_num_distinct_polys(); i++){
        auto polyrow = polyset.get_poly_at(i);
        std::cout << "row " << i << " ";
        for(auto& poly: polyrow){
            auto node = poly.head;
            for(unsigned int j=0; j<poly.size_ll; j++){
                const auto& pos = node->data.position;
                stream << "(" << pos.get_x() << "," << pos.get_y() << ") ";
                node = node->next;
            }
            stream << " | ";
        }
        stream << "\n";
    }
    
    return stream;
}


template <typename Poly>
struct Ostomini {
    static unsigned int max_size;
    static unsigned int num_distinct_polys;
    static Poly frame;
    static std::vector<std::pair<Poly, unsigned int>> pre_polys;
    static bool allow_reflection;
};

template <typename Poly> Poly Ostomini<Poly>::frame{{{-2,-2}, {-2,2}, {2,2}, {2,-2}}};
template <typename Poly> std::vector<std::pair<Poly, unsigned int>> Ostomini<Poly>::pre_polys{{
    {{{{0,0}, {2,4}, {0,4}}}, 2},
    {{{{0,0}, {1,0}, {1,2}}}, 2},
    {{{{1,0}, {3,0}, {3,2}, {1,2}}}, 1},
    {{{{1,2}, {3,2}, {2,4}}}, 1}
}};
template <typename Poly> unsigned int Ostomini<Poly>::max_size{6};
template <typename Poly> unsigned int Ostomini<Poly>::num_distinct_polys{static_cast<unsigned int>(Ostomini<Poly>::pre_polys.size())};
template <typename Poly> bool Ostomini<Poly>::allow_reflection{true};


template <typename Poly>
struct Ostomid {
    static unsigned int max_size;
    static unsigned int num_distinct_polys;
    static Poly frame;
    static Poly irreducible_section;
    static std::vector<std::pair<Poly, unsigned int>> pre_polys;
    static bool allow_reflection;
};

template <typename Poly> Poly Ostomid<Poly>::frame{{{-6,-6}, {-6,6}, {6,6}, {6,-6}}};
template <typename Poly> Poly Ostomid<Poly>::irreducible_section{{{-6,-6}, {0,-6}, {0,0}}};
template <typename Poly> std::vector<std::pair<Poly, unsigned int>> Ostomid<Poly>::pre_polys{{
    {{{{0,0},  {3,0},  {3,6 }}}, 1},
    {{{{3,0},  {6,0},  {6,6 }, {4,8}, {3,6}}}, 1},
    {{{{0,0},  {4,8},  {2,10}}}, 1},
    {{{{0,0},  {2,10}, {0,12}}}, 1},
    {{{{0,12}, {6,6},  {6,12}}}, 2},
    {{{{8,4},  {12,0}, {9,6 }}}, 1},
    {{{{9,6},  {12,0}, {12,8}}}, 1},
    {{{{6,6},  {8,4},  {9,6 },   {6,12}}}, 1},
    {{{{9,6},  {12,8}, {12,12},  {6,12}}}, 1}
}};
template <typename Poly> unsigned int Ostomid<Poly>::max_size{10};
template <typename Poly> unsigned int Ostomid<Poly>::num_distinct_polys{static_cast<unsigned int>(Ostomid<Poly>::pre_polys.size())};
template <typename Poly> bool Ostomid<Poly>::allow_reflection{true};








template <typename Poly>
struct Ostomed {
    static unsigned int max_size;
    static unsigned int num_distinct_polys;
    static Poly frame;
    static Poly irreducible_section;
    static std::vector<std::pair<Poly, unsigned int>> pre_polys;
    static bool allow_reflection;
};

template <typename Poly> Poly Ostomed<Poly>::frame{{{-6,-6}, {-6,6}, {6,6}, {6,-6}}};
template <typename Poly> Poly Ostomed<Poly>::irreducible_section{{{-6,-6}, {0,-6}, {0,0}}};
template <typename Poly> std::vector<std::pair<Poly, unsigned int>> Ostomed<Poly>::pre_polys{{
    {{{{0,0},  {3,0},  {3,6 }}}, 1},
    {{{{3,0},  {6,0},  {6,6 }, {4,8}, {3,6}}}, 1},
    {{{{0,0},  {4,8},  {2,10}}}, 1},
    {{{{0,0},  {2,10}, {0,12}}}, 1},
    {{{{0,12}, {4,8},  {6,12}}}, 2},
    {{{{4,8},  {6,6},  {6,12}}}, 2},
    {{{{8,4},  {12,0}, {9,6 }}}, 1},
    {{{{9,6},  {12,0}, {12,8}}}, 1},
    {{{{6,6},  {8,4},  {9,6 },   {6,12}}}, 1},
    {{{{9,6},  {12,8}, {12,12},  {6,12}}}, 1}
}};
template <typename Poly> unsigned int Ostomed<Poly>::max_size{12};
template <typename Poly> unsigned int Ostomed<Poly>::num_distinct_polys{static_cast<unsigned int>(Ostomed<Poly>::pre_polys.size())};
template <typename Poly> bool Ostomed<Poly>::allow_reflection{true};



template <typename Poly>
struct Rectangles {
    static unsigned int max_size;
    static unsigned int num_distinct_polys;
    static Poly frame;
    static std::vector<std::pair<Poly, unsigned int>> pre_polys;
    static bool allow_reflection;
};

template <typename Poly> Poly Rectangles<Poly>::frame{{{-2,-2}, {-2,2}, {2,2}, {2,-2}}};
template <typename Poly> std::vector<std::pair<Poly, unsigned int>> Rectangles<Poly>::pre_polys{{
    {{{{0,0}, {2,0}, {2,2}, {0,2}}}, 3},
    {{{{0,0}, {2,0}, {2,1}, {0,1}}}, 2}
}};
template <typename Poly> unsigned int Rectangles<Poly>::max_size{5};
template <typename Poly> unsigned int Rectangles<Poly>::num_distinct_polys{static_cast<unsigned int>(Rectangles<Poly>::pre_polys.size())};
template <typename Poly> bool Rectangles<Poly>::allow_reflection{false};




template <typename Poly>
struct Ostomachion {
    static unsigned int max_size;
    static unsigned int num_distinct_polys;
    static Poly frame;
    static Poly irreducible_section;
    static std::vector<std::pair<Poly, unsigned int>> pre_polys;
    static bool allow_reflection;
};

template <typename Poly> Poly Ostomachion<Poly>::frame{{{-6,-6}, {-6,6}, {6,6}, {6,-6}}};
template <typename Poly> Poly Ostomachion<Poly>::irreducible_section{{{-6,-6}, {0,-6}, {0,0}}};
template <typename Poly> std::vector<std::pair<Poly, unsigned int>> Ostomachion<Poly>::pre_polys{{
    {{{{0,0},  {3,0},  {2,4 }}}, 1},
    {{{{3,0},  {3,6},  {2,4 }}}, 1},
    {{{{3,0},  {6,0},  {6,6 }, {4,8}, {3,6}}}, 1},
    {{{{0,0},  {4,8},  {2,10}}}, 1},
    {{{{0,0},  {2,10}, {0,12}}}, 1},
    {{{{0,12}, {4,8},  {6,12}}}, 2},
    {{{{4,8},  {6,6},  {6,12}}}, 2},
    {{{{8,4},  {12,0}, {9,6 }}}, 1},
    {{{{9,6},  {12,0}, {12,6}}}, 1},
    {{{{9,6},  {12,6}, {12,8}}}, 1},
    {{{{6,6},  {8,4},  {9,6 },   {6,12}}}, 1},
    {{{{9,6},  {12,8}, {12,12},  {6,12}}}, 1}
}};
template <typename Poly> unsigned int Ostomachion<Poly>::max_size{14};
template <typename Poly> unsigned int Ostomachion<Poly>::num_distinct_polys{static_cast<unsigned int>(Ostomachion<Poly>::pre_polys.size())};
template <typename Poly> bool Ostomachion<Poly>::allow_reflection{true};