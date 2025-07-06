#pragma once

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstdlib>
#include <random>   
#include<curl/curl.h>
using namespace std;

#include "javalike_thread.hpp"

int randint(int min, int max){
    thread_local std::mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

static size_t discard_response(void* contents, size_t size, size_t nmemb, std::string* output) {
    /*size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;*/

    return size * nmemb;
}

class fetch_list {
    private:
        vector<string>& url_list;
        vector<bool> fetch_flags;

    public:
        fetch_list(vector<string>& p_url_list)
            :url_list(p_url_list), fetch_flags(url_list.size(), false)
        {
            //srand(time(NULL));
            if(url_list.empty()){
                throw runtime_error("Please provide a list with at least one url");
            }
        }

        string get_random_url(){
            int randnum;

            while(fetch_flags[randnum=randint(0, url_list.size()-1)]){}
            fetch_flags[randnum]=true;

            return url_list[randnum];

        }

        void reset(){
            for(int i=0; i<fetch_flags.size(); i++){
                fetch_flags[i]=false;
            }
        }
};

class testthread : public javalike_thread {
    private:
        fetch_list fetches;
        size_t iterations;
        size_t amount;

        string proxyurl;

    public:
        testthread(vector<string>& p_url_list, size_t p_iterations, size_t p_amount, string p_proxyurl)
            :fetches(p_url_list), iterations(p_iterations), amount(p_amount), proxyurl(p_proxyurl)
        {

            if(amount>p_url_list.size()){
                amount=p_url_list.size();
            }
        }

        void run(){
            CURL* curl;
            CURLcode res;

            curl = curl_easy_init();

            if(curl){
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_response);    //set callback to capture the response
                //curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);             //if not enabled: no need to store response

                curl_easy_setopt(curl, CURLOPT_PROXY, proxyurl.c_str());

                for(int i=0; i<iterations; i++){
                    for(int j=0; j<amount; j++){
                    	string tmp;
                        curl_easy_setopt(curl, CURLOPT_URL, (tmp=string("http://")+fetches.get_random_url()).c_str());          //set url to fetch
                        cout<<"URL: "<<tmp<<"\n";

                        res = curl_easy_perform(curl);

                        if (res != CURLE_OK) {
                            cerr << "["<<getname()<<"] curl_easy_perform() failed: "
                                    << curl_easy_strerror(res) << endl;
                        }
                    }
                
                    fetches.reset();
                }

                curl_easy_cleanup(curl);

            }else{
                cerr<<"["<<getname()<<"] Failed to init curl pointer.\n";
            }

            
        }
};