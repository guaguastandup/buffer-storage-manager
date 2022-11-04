#include <string>
#include <iostream>

// link: https://blog.csdn.net/jacke121/article/details/80026490
const char* c_str;

void test_string(std::string s) {
    c_str = s.c_str();
    std::cout<< s << " " << c_str << std::endl;
}

int main() {
    std::string s = "before";
    test_string(s);
    s = "after";
    std::cout<< s << " " << c_str << std::endl;
}
