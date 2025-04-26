#pragma once
#include <chrono>
#include <string>
#include <unordered_map>

class AnalyticsThread;

class TimingBranch {
private:
    unsigned long counter;
    std::chrono::high_resolution_clock::duration duration;
    std::chrono::high_resolution_clock::time_point t;

public:

    TimingBranch():
    counter{0},
    duration{0},
    builder{nullptr}{
        std::cout << "error\n";
        exit(1);
    };

    TimingBranch(AnalyticsThread* pointer):
        counter{0},
        duration{0},
        builder{pointer}{};

    void join(const TimingBranch& other){
        counter += other.counter;
        duration += other.duration;
    }

    void start(){
        t = std::chrono::high_resolution_clock::now();
    };
    void end(){
        duration += std::chrono::high_resolution_clock::now() - t;
        counter++;
    };

    double get_duration() const {
        return duration.count();
    }
    unsigned long get_counter() const {
        return counter;
    }
    
    AnalyticsThread* builder;

};

class AnalyticsThread {
public:
    TimingBranch& branch(const std::string& name){
        auto it = branches.find(name);
        if (it != branches.end())
            return it->second;
        else{
            TimingBranch new_branch(this);
            branches.insert({name, new_branch});
            return branches[name];
        }
        

    }
    void summary() const {
        for(const auto& [name, obj]: branches){
            std::cout << name << " " << obj.get_counter() << " " << obj.get_duration() << "\n";            
        }
    }

    void join(const AnalyticsThread& other) {
        for(const auto& [name, obj]: other.branches){
            branches[name].join(obj);
        }
    }


private:
    std::unordered_map<std::string, TimingBranch> branches;
};