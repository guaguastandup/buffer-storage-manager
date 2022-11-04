#include <iostream>
#include <string>
#include <cstring>

using namespace std;

#define MAXN 500
struct node {
    int value;
    
    char field[MAXN];

    node* next;
    node();
    node(int v, char* f) {
        this -> value = v;
        strcpy(field, f);
    }
};

int main() {
    node* x = new node(3, "12w");
    node* y = new node(2, "78s");

    strcpy(x->field, y->field);

    cout<< x->field << " " << y->field << endl;
}