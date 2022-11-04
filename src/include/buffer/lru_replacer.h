#include "include/storage/storage.h"
#include "include/buffer/replacer.h"

#include <mutex>

// TODO: implement of a DoubleLinkedList or not, emm...
class LRUReplacer: Replacer {
public:
    explicit LRUReplacer(size_t num_pages);
    ~LRUReplacer();

    bool Victim(int *frame_id) override;

    void Pin(int frame_id) override;
    void Unpin(int frame_id) override;

    void insert(Node *node);
    bool remove(int frame_id);
    void update(int frame_id);

    size_t Size() override;

private:
    std::mutex mtx;
    // DoubleLinkedList
    
    size_t size;
    size_t capacity;
    Node *head, *tail;
    // frame to Node*
    Node* fton[DEFBUF_SIZE];
};