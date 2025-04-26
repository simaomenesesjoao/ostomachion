#pragma once
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include "BarePoly.cpp"
#include "LLPoly.cpp"
#include "analytics.hpp"

namespace Polygon{


    template <typename Poly>
    Poly identity(const Poly& poly){
        return poly;
    };

    template <typename Poly>
    Poly rotate_90(const Poly& poly){
        Poly poly2 = poly;
        poly2.rotate({0,1},{0,0});
        return poly2;
    };

    template <typename Poly>
    Poly rotate_270(const Poly& poly){
        Poly poly2 = poly;
        poly2.rotate({0,-1},{0,0});
        return poly2;
    };

    template <typename Poly>
    Poly rotate_180(const Poly& poly){
        Poly poly2 = poly;
        poly2.rotate({-1,0},{0,0});
        return poly2;
    };

    template <typename Poly>
    Poly flip_x(const Poly& poly){
        Poly poly2 = poly;
        poly2.flip_x();
        return poly2;
    };

    template <typename Poly>
    Poly flip_y(const Poly& poly){
        Poly poly2 = poly;
        poly2.flip_y();
        return poly2;
    };

    template <typename Poly>
    Poly flip_D(const Poly& poly){
        return rotate_90(flip_x(poly));
    };

    template <typename Poly>
    Poly flip_AD(const Poly& poly){
        return rotate_270(flip_x(poly));
    };


    enum class Transformations {Id, Rot, Flip, RotFlip, R90};

    template <typename Num>
    class ContigPoly {
    public:
        ContigPoly(const BarePoly& poly){};
    };



    template <typename Poly>
    class RestrictedPoly {
        void apply_transformation(){
            // Applies all transformations to one of the polygons
            Poly poly = variations_.front();
            variations_.clear();

            switch(transformation_){
                case Transformations::Id:
                    break;

                case Transformations::Rot:

                    for(unsigned int i=0; i<poly.size(); i++){
                        variations_.push_back(poly);
                        poly.advance_head();                   
                    }
                    
                    break;

                case Transformations::Flip:
                    variations_.push_back(poly);
                    poly.flip_x();
                    variations_.push_back(poly);
                    break;

                case Transformations::RotFlip:

                    for(unsigned int i=0; i<poly.size(); i++){
                        variations_.push_back(poly);
                        poly.advance_head();                    
                    }
                    poly.flip_x();
                    for(unsigned int i=0; i<poly.size(); i++){
                        variations_.push_back(poly);
                        poly.advance_head();                      
                    }

                    break;

                case Transformations::R90:

                    for(unsigned int i=0; i<2; i++){
                        variations_.push_back(poly);
                        poly.advance_head();                    
                    }

                    break;
            }
        }
    public:
        RestrictedPoly(const std::vector<Poly>& variations): 
            variations_{variations}, num_copies_{1}, transformation_{Transformations::RotFlip}{
                apply_transformation();
            };

        RestrictedPoly(const Poly& poly): RestrictedPoly(std::vector<Poly>{poly}){};

        RestrictedPoly& num_copies(unsigned int n){
            num_copies_ = n;
            return *this;
        }

        RestrictedPoly& restriction(const Poly& poly){
            restriction_ = poly;
            return *this;
        }

        RestrictedPoly& transformation(Transformations t){
            transformation_ = t;
            apply_transformation();
            return *this;
        }


        Transformations get_transformation() const {
            return transformation_;
        }

        const std::vector<Poly>& get_variations() const {
            return variations_;
        }

        const Poly& get_restriction() const {
            return restriction_;
        }

        unsigned int get_amount() const {
            return num_copies_;
        }

    private:
        std::vector<Poly> variations_;
        unsigned int num_copies_;
        Poly restriction_;
        Transformations transformation_;
    };

    template <typename Poly>
    class Pool {
    public:
        std::vector<RestrictedPoly<Poly>> pool;
        unsigned int size;

        Pool(): pool{}, size{0}{}

        Pool(const std::vector<RestrictedPoly<Poly>>& init_pool): pool{init_pool}, size{0}{
            for(auto& poly: pool){
                size += poly.get_amount();
            }
        }

        template <typename OtherPoly>
        Polygon::Pool<OtherPoly> convert() const {
            // std::cout << "converting poly pool:\n";

            // std::cout << "BarePoly pool:\n";

            // for(const auto& rp: pool){
            //     std::cout << "restricted poly variations:\n";
            //     for(const auto& poly: rp.get_variations()){
            //         std::cout << "poly: ";
            //         for(const auto& vertex: poly.point_list){
            //             std::cout << "(" << vertex.at(0) << "," << vertex.at(1) << ")";
            //         }
            //         std::cout << "\n";
            //     }
            // }

            
            Polygon::Pool<OtherPoly> converted_pool;
            converted_pool.size = size;
            for(const auto& restricted_poly: pool){

                std::vector<OtherPoly> variations;
                for(const auto& var: restricted_poly.get_variations()){

                    // std::cout << "before conversion:\n";
                    // for(const auto& vertex: var.point_list){
                    //     std::cout << "(" << vertex.at(0) << "," << vertex.at(1) << ")";
                    // }
                    
                    variations.push_back(OtherPoly(var));
                    // std::cout << "after conversion:\n";
                    // OtherPoly(var).print();
                    // std::cout << "finished ind conv\n";
                }

                OtherPoly restriction(restricted_poly.get_restriction());
                unsigned int copies = restricted_poly.get_amount();
                Transformations t = restricted_poly.get_transformation();

                RestrictedPoly<OtherPoly> converted_rpoly(variations);
                converted_rpoly.num_copies(copies).restriction(restriction).transformation(t);
                converted_pool.pool.push_back(converted_rpoly);

            }

            // std::cout << "after conversion:\n";
            // for(const auto& rp: converted_pool.pool){
            //     std::cout << "restricted poly variations:\n";
            //     for(const auto& poly: rp.get_variations()){
            //         poly.print();
            //         std::cout << "\n";
            //     }
            // }
            // std::cout << "done\n";

            return converted_pool;
        }
    };

        
    template <typename Poly>
    std::function<bool(const Poly&, const Poly&, TimingBranch&)> overlapper_factory(const std::string& name){
        
        if(name == "edge"){
            return [](const Poly& poly1, const Poly& poly2, TimingBranch& timing){ 
                timing.start();
                bool intersects = poly1.edge_edge_intersection(poly2);
                timing.end();
                return intersects;
            };
        } else if(name == "complete"){
            return [](const Poly& poly1, const Poly& poly2, TimingBranch& timing){ 
                timing.start();
                bool overlaps = poly1.overlaps(poly2);
                timing.end();
                return overlaps;
            };
        } else if(name == "no-op"){
            return [](const Poly&, const Poly&, [[maybe_unused]] TimingBranch& timing){ 
                return true;
            };
        } else {
            assert(false); // SIMAO: substituir por exception
        }
    }


    template <typename Poly>
    std::function<unsigned int (const Poly&)> selector_factory(const std::string& name){
        
        if(name == "farthest"){
            return [](const Poly& poly){ 
                return poly.get_farthest_node(0,0);
            };
        } else if(name == "obtusest") {
            return [](const Poly& poly){ 
                return poly.get_obtusest_node();
            };
        } else if(name == "leftest") {
            return [](const Poly& poly){ 
                return poly.get_leftest_node();
            };
        } else {
            assert(false); // SIMAO: substituir por exception
        }
    }


}