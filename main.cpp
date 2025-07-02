#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <cctype>
#include <curl/curl.h>

#include "testthread.hpp"

using namespace std;

//WRITTEN ENTIRELY BY CHATGPT, STILL TO TEST

// Utility to trim whitespace
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

// Utility to get validated numeric input
template<typename T>
T get_validated_input(const string& prompt) {
    T value;
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        input = trim(input);
        try {
            if constexpr (is_same<T, size_t>::value) {
                if (input.empty() || input.find_first_not_of("0123456789") != string::npos)
                    throw invalid_argument("Invalid number");
                value = static_cast<size_t>(stoull(input));
                if (value == 0) throw invalid_argument("Must be > 0");
            } else {
                throw invalid_argument("Unsupported type");
            }
            break;
        } catch (...) {
            cout << "Invalid input. Please enter a positive number.\n";
        }
    }
    return value;
}

int main() {
    string url_filename;
    string proxy_url;

    // Get URL filename
    while (true) {
        cout << "Enter URL list filename: ";
        getline(cin, url_filename);
        url_filename = trim(url_filename);
        if (!url_filename.empty()) break;
        cout << "Filename cannot be empty.\n";
    }

    // Get proxy URL
    while (true) {
        cout << "Enter proxy URL: ";
        getline(cin, proxy_url);
        proxy_url = trim(proxy_url);
        if (!proxy_url.empty()) break;
        cout << "Proxy URL cannot be empty.\n";
    }

    // Get numeric parameters
    size_t num_urls_to_fetch = get_validated_input<size_t>("Enter number of URLs to fetch per iteration: ");
    size_t num_iterations = get_validated_input<size_t>("Enter number of iterations: ");
    size_t num_threads = get_validated_input<size_t>("Enter number of threads: ");

    // Load URLs from file
    vector<string> url_list;
    ifstream url_file(url_filename);
    string line;

    if (!url_file.is_open()) {
        cerr << "Failed to open URL file: " << url_filename << endl;
        return 1;
    }

    while (getline(url_file, line)) {
        line = trim(line);
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
        try {
            auto t = make_unique<testthread>(url_list, num_iterations, num_urls_to_fetch, proxy_url);
            t->setname("Thread-" + to_string(i));
            t->start();
            threads.push_back(move(t));
        } catch (const exception& e) {
            cerr << "Thread-" << i << " initialization failed: " << e.what() << endl;
        }
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
