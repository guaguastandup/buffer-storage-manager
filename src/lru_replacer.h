#ifndef LRU_REPLACER
#define LRU_REPLACER
#include "storage.h"
// #include "replacer.h"

#include <mutex>

struct Node {
    explicit Node(int frame_id): val(frame_id) {}
    int val;
    Node* nxt = nullptr;
    Node* pre = nullptr;
};

// class LRUReplacer: public Replacer {
class LRUReplacer{
public:
    explicit LRUReplacer(size_t num_pages);
    ~LRUReplacer();

    bool Victim(int *frame_id);

    void Pin(int frame_id);
    void Unpin(int frame_id);


    void insert(Node *node);
    bool remove(int frame_id);
    void update(int frame_id);

    // size_t Size() override;
    size_t Size();

private:
    std::mutex mtx;
    
    size_t size;
    size_t capacity;
    Node *head, *tail;

    // frame to Node*
    Node* fton[DEFBUF_SIZE];
};

#endif
