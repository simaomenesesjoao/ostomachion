#pragma once

#include "polygon.cpp"
#include <functional>
#include <vector>

template <typename Poly>
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

    virtual unsigned int get_max_size() const = 0;
    virtual unsigned int get_num_distinct_polys() const = 0;    
    virtual void calculate_hash() = 0;

    unsigned long long get_hash() const {
        return hash_;
    };


    const std::vector<Poly>& get_poly_at(unsigned int i) const {
        return polygons.at(i);
    }
};

template <typename Poly>
std::ostream& operator<<(std::ostream& stream, const PolySet<Poly>& polyset){
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

template <typename Poly>
class Ostomini: public PolySet<Poly> {
private:
    static std::vector<std::pair<Poly, unsigned int>> polyset;
    static Poly frame;



    bool equals(const PolySet<Poly>& other, std::function<Poly(const Poly&)> transform) const {
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

    
    static Poly identity(const Poly& poly){
        return poly;
    }

    static Poly rotation90(const Poly& poly){
        Poly poly2 = poly;
        poly2.rotate({0,1},{2,2});
        return poly2;
    }

    static Poly rotation270(const Poly& poly){
        Poly poly2 = poly;
        poly2.rotate({0,-1},{2,2});
        return poly2;
    }

    static Poly rotation180(const Poly& poly){
        Poly poly2 = poly;
        poly2.rotate({-1,0},{2,2});
        return poly2;
    }

    static Poly flip_x(const Poly& poly){
        Poly poly2 = poly;
        poly2.flip_x();
        poly2.translate({4,0});
        return poly2;
    }

    static Poly flip_y(const Poly& poly){
        Poly poly2 = poly;
        poly2.flip_y();
        poly2.translate({0,4});
        return poly2;
    }

    std::vector<std::function<Poly(const Poly&)>> transformations = {identity, rotation90, rotation180, rotation270, flip_x, flip_y};
    

public:
    static unsigned int max_size;
    static unsigned int num_distinct_polys;

    Ostomini():PolySet<Poly>(polyset.size()){}

    
    unsigned int get_max_size() const override {
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
    }

    bool operator==(const Ostomini& other) const {

        int count = 0;
        for(auto& f: transformations){
            count++;
            if(equals(other, f))
                return true;
        }
        return false;
    }

    void calculate_hash(){
        // Calculates a hash that is invariant under rotation and reflection
        unsigned long long int h = 0;
        for(auto& polyrow: this->polygons){
            for(auto& poly: polyrow){
                for(auto& f: transformations){
                    h += 3991*f(poly).get_hash();
                    h = h%94619837451;
                }
            }
        }
        this->hash_ = h;

    }

};


template <typename Poly> 
Poly Ostomini<Poly>::frame{{0,0}, {0,4}, {4,4}, {4,0}};

template <typename Poly> 
std::vector<std::pair<Poly, unsigned int>> Ostomini<Poly>::polyset{{
    {{{{0,0}, {2,4}, {0,4}}}, 2},
    {{{{0,0}, {1,0}, {1,2}}}, 2},
    {{{{1,0}, {3,0}, {3,2}, {3,1}}}, 1},
    {{{{1,2}, {3,2}, {2,4}}}, 1}
}};

template <typename Poly>
unsigned int Ostomini<Poly>::max_size = 6;
template <typename Poly>
unsigned int Ostomini<Poly>::num_distinct_polys = polyset.size();


