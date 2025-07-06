#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <curl/curl.h>

#include "testthread.hpp"

using namespace std;

int main() {
    string url_filename;
    string proxy_url;
    size_t num_urls_to_fetch;
    size_t num_iterations;
    size_t num_threads;

    // Get user input
    cout << "Enter URL list filename: ";
    getline(cin, url_filename);

    cout << "Enter proxy URL: ";
    getline(cin, proxy_url);

    cout << "Enter number of URLs to fetch per iteration: ";
    cin >> num_urls_to_fetch;

    cout << "Enter number of iterations: ";
    cin >> num_iterations;

    cout << "Enter number of threads: ";
    cin >> num_threads;

    // Load URLs from file
    vector<string> url_list;
    ifstream url_file(url_filename);
    string line;

    if (!url_file.is_open()) {
        cerr << "Failed to open URL file: " << url_filename << endl;
        return 1;
    }

    while (getline(url_file, line)) {
        if (!line.empty())
            url_list.push_back(line);
    }

    if (url_list.empty()) {
        cerr << "URL file is empty or contains only blank lines." << endl;
        return 1;
    }

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);

    // Create threads
    vector<unique_ptr<testthread>> threads;

    for (size_t i = 0; i < num_threads; ++i) {
        auto t = make_unique<testthread>(url_list, num_iterations, num_urls_to_fetch, proxy_url);
        t->setname("Thread-" + to_string(i));
        t->start();
        threads.push_back(move(t));
    }

    // Join threads
    for (auto& t : threads) {
        t->join();
    }

    // Cleanup libcurl
    curl_global_cleanup();

    cout << "All threads completed." << endl;
    return 0;
}
