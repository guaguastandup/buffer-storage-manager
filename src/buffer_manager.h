#ifndef BUFFER_MANAGER
#define BUFFER_MANAGER
// #include "include/storage/storage.h"
#include "storage_manager.h"
#include "lru_replacer.h"

#include <vector>
#include <list>

class BMgr {
public:
    BMgr(DSMgr *dsMgr, LRUReplacer* replacer);
    virtual ~BMgr();
    // Interface functions
    int FixPage(int page_id, int prot);
    // void NewPage FixNewPage();
    void FixNewPage();
    int UnfixPage(int page_id);

    int NumFreeFrames();

    // Internal Functions
    int SelectVictim();
    
    int Hash(int page_id);
    
    void RemoveBCB(BCB * ptr, int page_id);
    void RemoveLRUEle(int frid);
    BCB * GetBCB(int page_id);

    void SetDirty(int frame_id);
    void UnsetDirty(int frame_id);
    void WriteDirtys();
    
    void PrintFrame(int frame_id); // prints out the contents of the frame described by the frame_id.
    void IncHitCnt() { hitCnt+=1; }
    int GetHitCnt() { return hitCnt; }
    
private:
    BCB* buffer[DEFBUF_SIZE]; // indexed by frame_id
    BCB* ptob[DEFBUF_SIZE];
    bFrame frames[DEFBUF_SIZE]; 
    std::list<int> freeList; // store the free frame id
    int pages[TOTAL_PAGE_NUM]; // pages[page_id] = frame_id

    DSMgr *dsMgr;
    LRUReplacer *replacer;
    int hitCnt;
};

#endif