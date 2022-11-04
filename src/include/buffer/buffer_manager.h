#include "include/storage/storage.h"
#include "include/storage/storage_manager.h"
#include "include/buffer/replacer.h"
#include "include/buffer/lru_replacer.h"

#include <vector>
#include <list>

class BMgr {
public:
    BMgr(DSMgr *dsMgr, Replacer* replacer);

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
    
    void SetDirty(int frame_id);
    void UnsetDirty(int frame_id);
    void WriteDirtys();
    
    void PrintFrame(int frame_id); // prints out the contents of the frame described by the frame_id.
    void IncHitCnt() { hitCnt+=1; }
    void IncPageUpdateCnt() { pageUpdateCnt+=1; }

private:

    /**
     * bcbBuffer[frame_id] = BCB{page_id, frame_id, dirty, count, ...} 
     * the data of this frame_id is frames[frame_id].field
     * this is the real buffer, because it contains the detailed infomation of pages usage
     **/
    BCB* buffer[DEFBUF_SIZE]; // indexed by frame_id
    std::list<int> freeList; // store the free frame id

    // Hash Table
    int ftop[DEFBUF_SIZE];  // frame_id to page_id
    BCB* ptob[DEFBUF_SIZE]; // page_id to BCB(contains frame_id)

    int pages[TOTAL_PAGE_NUM]; // pages[page_id] = frame_id

    DSMgr *dsMgr;
    Replacer *replacer;

    int hitCnt;
    int pageUpdateCnt;

};
