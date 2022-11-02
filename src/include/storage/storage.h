#define FRAMESIZE 4096
#define DEFBUFSIZE 1024
#define BUFFERSIZE 1024
#define MAXPAGES 2048

struct bFrame {
    char field[FRAMESIZE]; 
};

struct BCB {
    BCB();
    int page_id;    // 虚拟page
    int frame_id;   // 物理
    int latch;      // 锁
    int count;
    int dirty; 
    BCB * next;     // 块是顺序相连的
};


bFrame buf[DEFBUFSIZE]; // or the size that the user defined by the input parameter

BCB hPageBCBTable[BUFFERSIZE]; // page_id to BCB

int hFrameBCBPTable[BUFFERSIZE]; // frame_id to page_id
// page_id & page_size are the most important

