#pragma once

#include <iostream>
#include <thread>
#include <memory>
using namespace std;

class javalike_thread {
    private:
        std::thread t;
        string name;

    public:
        javalike_thread(string p_name="javalike_thread")
            :name(p_name)
        {
        }

        virtual ~javalike_thread() = default;

        void start() {
            t = thread(&javalike_thread::run, this);
        }

        void join() {
            if (t.joinable())
                t.join();
        }

        string getname(){
            return name;
        }

        void setname(string p_name){
            name=p_name;
        }

    protected:
        virtual void run() = 0;
};
