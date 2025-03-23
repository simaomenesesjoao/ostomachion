#pragma once

#include "polygon.cpp"
#include <functional>
#include <vector>

template <typename Poly, template <typename> class Group>
class PolySet {


protected:

    unsigned long long hash_;
    std::vector<std::vector<Poly>> polygons;
    unsigned int size_;

    PolySet(unsigned int i):polygons(i), size_{0}{};

public:
    virtual void insert(unsigned int i, const Poly& poly) = 0;

    unsigned int get_size() const {
        return size_;
    }

    // virtual unsigned int get_max_size() const = 0;
    virtual unsigned int get_num_distinct_polys() const = 0;    
    virtual void calculate_hash() = 0;

    unsigned long long get_hash() const {
        return hash_;
    };


    const std::vector<Poly>& get_poly_at(unsigned int i) const {
        return polygons.at(i);
    }

    std::vector<std::vector<std::vector<double>>> as_vector() const {
        std::vector<std::vector<std::vector<double>>> polygon_vector;
        for(const auto& polyrow: polygons)
            for(const auto& poly: polyrow)
                polygon_vector.push_back(poly.as_vector());
        return polygon_vector;
    }
};

template <typename Poly, template <typename> class Group>
std::ostream& operator<<(std::ostream& stream, const PolySet<Poly, Group>& polyset){
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

// template <typename Poly>
// class Ostomachion: public PolySet<Poly> {
// };

template <typename Poly, template <typename> class Group>
class Ostomini: public PolySet<Poly, Group> {
private:



    bool equals(const PolySet<Poly, Group>& other, std::function<Poly(const Poly&)> transform) const {
        // Compare the positions of each set of polygons. Returns true if all
        // of them are identical. Allows a transformation to be applied to the 
        // first set, so that two sets can be identical if they differ by 
        // rotation or reflection


        if(this->get_size() != other.get_size())
            return false;

        for(unsigned int i = 0; i < num_distinct_polys; i++){
            auto& type = this->polygons.at(i);
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
    
    static unsigned int max_size;
    static unsigned int num_distinct_polys;

public:
    static Poly frame;
    static std::vector<std::pair<Poly, unsigned int>> polyset;

    void calculate_hash(){
        // Calculates a hash that is invariant under rotation and reflection
        unsigned long long int h = 0;

        // int countf = 0;
        for(auto& f: Group<Poly>::transformations){
            // std::cout << "transformation " << countf << "\n";
            // countf++;
            // int count = 0;
            for(auto& polyrow: this->polygons){
                for(auto& poly: polyrow){
                    unsigned long long h_ = 3991*f(poly).get_hash();
                    // Poly poly2 = f(poly);
                    // std::cout << count << " ";
                    // for(const auto& node: poly2.as_vector()){
                    //     std::cout << "(" << node.at(0) << "," << node.at(1) << ") ";
                    // }
                    // std::cout << " " << h_ << "\n";
                    h += h_;
                    h = h%94619837451;
                    // count++;
                }
            }
        }
        this->hash_ = h;
    }
    Ostomini():PolySet<Poly, Group>(num_distinct_polys){
        calculate_hash();
    }


    bool is_type_available(unsigned int i) const {
        return this->polygons.at(i).size() < polyset.at(i).second;
    }

    std::vector<unsigned int> find_available_types() const {
        // returns a vector with the indices of all the polygon types
        // that are not complete yet
        std::vector<unsigned int> available_types;
        for(unsigned i=0; i<num_distinct_polys; i++){
            if(this->polygons.at(i).size() < polyset.at(i).second)
            available_types.push_back(i);
        }
        return available_types;
    }

    static unsigned int get_max_size(){
        return max_size;
    }
    
    unsigned int get_num_distinct_polys() const override {
        return num_distinct_polys;
    }
    
    void insert(unsigned int i, const Poly& poly) override {
        auto& polyrow = this->polygons.at(i);
        assert(poly.size_ll == polyset.at(i).first.size_ll);
        assert(polyrow.size() < polyset.at(i).second);
        polyrow.push_back(poly);
        calculate_hash();
        this->size_++;
    }

    bool operator==(const Ostomini& other) const {

        int count = 0;
        for(auto& f: Group<Poly>::transformations){
            count++;
            if(equals(other, f))
                return true;
        }
        return false;
    }

    bool strict_equality(const Ostomini& other) const {
        return equals(other, [](const Poly& poly){return poly;});
    }

};


// template <typename Poly> 
// Poly Ostomini<Poly>::frame{{{-3,-3}, {-3,3}, {3,3}, {3,-3}}};


template <typename Poly, template <typename> class Group> 
Poly Ostomini<Poly, Group>::frame{{{-2,-2}, {-2,2}, {2,2}, {2,-2}}};

// template <typename Poly> 
// Poly Ostomini<Poly>::frame{{{0,0}, {0,4}, {4,4}, {4,0}}};

// template <typename Poly> 
// std::vector<std::pair<Poly, unsigned int>> Ostomini<Poly>::polyset{{
//     {{{{0,0}, {2,4}, {0,4}}}, 2},
//     {{{{0,0}, {1,0}, {1,2}}}, 2},
//     {{{{1,0}, {3,0}, {3,2}, {1,2}}}, 1},
//     {{{{1,2}, {3,2}, {2,4}}}, 1}
// }};

// template <typename Poly>
// unsigned int Ostomini<Poly>::max_size = 6;


// template <typename Poly> 
// std::vector<std::pair<Poly, unsigned int>> Ostomini<Poly>::polyset{{
//     {{{{0,0}, {2,0}, {2,1}, {0,1}}}, 8}
// }};

// template <typename Poly>
// unsigned int Ostomini<Poly>::max_size = 8;




template <typename Poly, template <typename> class Group> 
std::vector<std::pair<Poly, unsigned int>> Ostomini<Poly, Group>::polyset{{
    {{{{0,0}, {1,0}, {1,1}, {0,1}}}, 12},
    {{{{0,0}, {2,0}, {2,1}, {0,1}}}, 2}
}};

template <typename Poly, template <typename> class Group>
unsigned int Ostomini<Poly, Group>::max_size = 14;







template <typename Poly, template <typename> class Group>
unsigned int Ostomini<Poly, Group>::num_distinct_polys = polyset.size();