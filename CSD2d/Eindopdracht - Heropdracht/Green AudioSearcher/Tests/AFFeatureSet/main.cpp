#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include "../Source/AFFeatureSet.h"

int main() {
    
    // building featuresets:
    std::cout << "test building featuresets: ";
    try {
        std::vector<std::string> s = {"0", "1", "2"};
        std::vector<float> f = {0, 1, 2};
        AFFeatureSet set("dropbox/samples/example.wav", "v0.1", s, f);
        std::string s_set = set.toString();
        std::cout << "succes" << std::endl;
        std::cout << "\toutput: " << s_set << std::endl;
    } 
    catch(std::exception& e) {
        std::cout << "failed" << std::endl;    
        std::cout << "\toutput: " << e.what() << '\n';
    }
    
    // building featuresets with incorrect input (1):
    std::cout << "test building featuresets with incorrect input (1): ";
    try {
        std::vector<std::string> s = {"0", "1"};
        std::vector<float> f = {0, 1, 2};
        AFFeatureSet set("dropbox/samples/example.wav", "v0.1", s, f);
        std::string s_set = set.toString();
        std::cout << "failed: set build" << std::endl;
        std::cout << "\toutput: " << s_set << std::endl;
    } 
    catch(std::exception& e) {
        std::cout << "succes" << std::endl;    
        std::cout << "\toutput: " << e.what() << '\n';
    }
    
    // building featuresets with incorrect input (2):
    std::cout << "test building featuresets with incorrect input (2): ";
    try {
        std::vector<std::string> s = {"0", "1", "2"};
        std::vector<float> f = {0, 1};
        AFFeatureSet set("dropbox/samples/example.wav", "v0.1", s, f);
        std::string s_set = set.toString();
        std::cout << "failed: set build" << std::endl;
        std::cout << "\toutput: " << s_set << std::endl;
    } 
    catch(std::exception& e) {
        std::cout << "succes" << std::endl;    
        std::cout << "\toutput: " << e.what() << '\n';
    }
};