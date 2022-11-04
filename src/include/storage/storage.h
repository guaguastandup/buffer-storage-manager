#include <unordered_map>
#include <cstring>

#define DEFBUF_SIZE 1024
#define BUFFER_SIZE 1024

#define FRAME_SIZE 4096
#define PAGE_SIZE 4096
#define TOTAL_PAGE_NUM 50001
#define CONTENT_ITEM_SIZE 4

// 4096/4 = 1024
const int CONTENT_MAX_SIZE = (PAGE_SIZE / CONTENT_ITEM_SIZE); 
const int PAGE_NUM_IN_CONTENT = (CONTENT_MAX_SIZE - 1); // 1023

struct bFrame {
    bFrame();
    bFrame(char * value) {
        strcpy(this->field, value);
    }
    char field[FRAME_SIZE]; // 单个frame的size
};

// buffer control block
struct BCB {
    BCB();
    BCB(int page_id, int frame_id) {
        this->page_id = page_id;
        this->frame_id = frame_id;
        this->latch = 0;
        this->dirty = 0;
        this->count = 0;
        this->next = nullptr;
    }

    int page_id; 
    int frame_id;
    int latch;      
    int count;
    int dirty; 
    BCB * next;     // 块是顺序相连的
};
    

BCB* findByPageID(BCB* bcb, int page_id) {
    while(bcb != nullptr) {
        if(bcb->page_id == page_id) {
            return bcb;
        } 
        bcb = bcb -> next;
    }
    return bcb;
}

/** buffer size is 1024 by default
 * The buffer consists of an array of frames.
 * we make the frame size the same as the size of a page 
 * in order to prevent external fragmentation.
*/
bFrame frames[DEFBUF_SIZE]; 

std::unordered_map<int, int> page_usebit;

// 本项目中page's format 不用被定义
// 只需要page_size(same as framesize) 和  page_id
// page_id在磁盘中, 是实际的物理id
// frame_id在内存中, 是buffer中的下标