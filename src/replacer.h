#ifndef REPLACER
#define REPLACER
// struct Node {
//     explicit Node(int frame_id): val(frame_id) {}
//     int val;
//     Node* nxt = nullptr;
//     Node* pre = nullptr;
// };

class Replacer {
public:
    // Replacer() = default;
    Replacer();
    ~Replacer();
    // virtual ~Replacer() noexcept;

    virtual bool Victim(int *frame_id);

    virtual void Pin(int frame_id) = 0;
    virtual void Unpin(int frame_id) = 0;
    
    virtual size_t Size();
};

#endif