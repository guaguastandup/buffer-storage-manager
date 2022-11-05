#include "lru_replacer.h"
#include <iostream>

LRUReplacer::LRUReplacer(size_t buf_size) {
    head = new Node(-1);
    tail = new Node(-1);
    // tail = head;
    head->nxt = tail;
    tail->pre = head;
    capacity = buf_size;
    size = 0;
}

LRUReplacer::~LRUReplacer() {
    while(size != 0) {
        remove(head->nxt->val);
    }
    delete head;
    delete tail;
}

bool LRUReplacer::Victim(int *frame_id) {
    if(Size() == 0) { // no victim page, still got free buffer
        *frame_id = -1;
        return false;
    }
    Node *node = tail->pre; // 选择队尾元素 pop_back
    *frame_id = node->val;
    remove(node->val);
    return true;
}

void LRUReplacer::Pin(int frame_id) { // write frame to disk
    if(fton[frame_id] != nullptr) {
        remove(frame_id);
    }
}

void LRUReplacer::Unpin(int frame_id) {
    if(Size() >= capacity) {
        std::cout<< "victim start"<<std::endl;
        while(Size() >= capacity) {
            Node *node = tail->pre;
            // the deleted node is the victim
            std::cout<< "victim: "<<node->val<<std::endl;
            remove(node->val); // this is not in the queue, so the BCB will be deleted
        }
         std::cout<< "victim end\n"<<std::endl;
    }
    if(fton[frame_id] != nullptr) {
        remove(frame_id);
    }
    Node *node = new Node(frame_id);
    insert(node);
}

void LRUReplacer::insert(Node *node) { // 添加到队首, 因为是最近使用的
    if(node == nullptr) {
        return;
    }
    node->pre = head;
    node->nxt = head->nxt;
    head->nxt->pre = node;
    head->nxt = node;
    fton[node->val] = node;
    size++;
}

bool LRUReplacer::remove(int frame_id) { // 指定frame的元素
    if(fton[frame_id] == nullptr) {
        return false;
    }
    Node* node = fton[frame_id];
    node->pre->nxt = node->nxt;
    node->nxt->pre = node->pre;
    fton[node->val] = nullptr;
    size--;
    delete node;
    return true;
}

size_t LRUReplacer::Size() {
    return size;
}