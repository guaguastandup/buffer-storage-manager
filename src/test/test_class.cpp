#include <iostream>

using namespace std;

struct node {
    int value;
    node* next;
    node();
    node(int v) {
        this -> value = v;
    }
};

node * f(node * x, int value) {
    while(x != nullptr) {
        if(x->value == value) {
            return x;
        }
        x = x -> next;
    } 
    return x;
}

void test1() {
    node* x = new node(3);
    x -> next = new node(4);    
    cout<< "x: " << x << endl;
    cout<< "x_next: " << x -> next << endl;
    node* y = f(x, 4);
    cout<< "x: " << x << endl;
    cout<< "x_next: " << x -> next << endl;
    cout<< "y: " << y << endl;
}

void f1(node* x) {
    x = x->next;
}

void test2() {
    node* x = new node(3);
    x->next = new node(4);
    f1(x);

    cout<<x->value<< "," << x->next->value<<endl;
}

node* hmap[10];

node* test3(node* x) {
    return x;
}


int main() {
    node* x = new node(3);
    node* y = new node(5);
    hmap[0] = x;

    x->next = y;

    x = test3(x);

    x->value -= 1;

    cout<<hmap[0]->value<<endl;
}