#ifndef STORAGE
#define STORAGE

#include <assert.h>
#include <unordered_map>
#include <cstring>

// #define DEFBUF_SIZE 1024
// #define BUFFER_SIZE 1024

#define DEFBUF_SIZE 512
#define BUFFER_SIZE 512

#define FRAME_SIZE 4096
#define PAGE_SIZE 4096
#define TOTAL_PAGE_NUM 50001

struct bFrame {
    bFrame() { }
    bFrame(char * value) {
        strcpy(this->field, value);
    }
    char field[FRAME_SIZE]; // 单个frame的size
};

// buffer control block
struct BCB {
    BCB();
    BCB(int page_id, int frame_id = -1) { // 如果该page不在buffer中, 则设置frame_id为-1
        this->page_id = page_id;
        this->frame_id = frame_id;
        this->latch = 0;
        this->dirty = 0;
        this->count = 0;
        this->next = nullptr;
    }
    int page_id; 
    int frame_id = -1;
    int latch;      
    int count;
    int dirty; 
    // BCB * prev;     // 块是顺序相连的
    BCB * next;     // 块是顺序相连的
    // BCB * tail;     // 块是顺序相连的
};

#endif