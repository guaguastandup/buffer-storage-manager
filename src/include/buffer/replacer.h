
struct Node {
    explicit Node(int frame_id): val(frame_id) {}
    int val;
    Node* nxt = nullptr;
    Node* pre = nullptr;
};

class Replacer {
public:
    Replacer() = default;
    virtual ~Replacer() = default;

    virtual auto Victim(int *frame_id) -> bool = 0;

    virtual void Pin(int frame_id) = 0;
    virtual void Unpin(int frame_id) = 0;

    virtual auto Size() -> size_t = 0;
};