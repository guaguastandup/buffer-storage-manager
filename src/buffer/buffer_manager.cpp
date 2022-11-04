#include "include/storage/storage.h"
#include "include/buffer/buffer_manager.h"

BMgr::BMgr(DSMgr *dsMgr, Replacer* replacer) {
    this->dsMgr = dsMgr;
    // this->replacer = new LRUReplacer(DEFBUF_SIZE);
    this->replacer = replacer;
    // init freeList
    freeList.clear();
    for(int i=0; i<DEFBUF_SIZE; i++) {
        this -> freeList.emplace_back(i);
    }
    memset(pages, -1, sizeof(pages));
}

BMgr::~BMgr() {
    delete replacer;
    delete[] buffer;
}

// Interface functions
// 读取page_id里的内容, 返回frame_id
int BMgr::FixPage(int page_id, int prot) {
    int frame_id = Hash(page_id);
    BCB* ptr = findByPageID(ptob[frame_id], page_id); // find BCB by page_id
    if(ptr != nullptr) {
        IncHitCnt();
        // lruReplacer -> remove(frame_id);
        // lruReplacer -> Unpin(frame_id);
        return ptr->frame_id; // pin the page
    } else { // no such bcb of the page
        // 创建新的bcb
        ptr = ptob[frame_id]; // get the bcb
        int victim_id = SelectVictim();
        //TOFIX
        strcpy(frames[victim_id].field, dsMgr->ReadPage(page_id).field);
        ftop[victim_id] = page_id;
        if(ptr != nullptr) {
            while(ptr->next != nullptr) {
                ptr = ptr->next;
            }
            ptr -> next = new BCB(page_id, victim_id); // create a new BCB
            ftop[frame_id] = page_id; // frame_id to page_id
        } else {
            ptr = new BCB(page_id, victim_id); // create a new BCB
            ptob[page_id] = ptr;  // page_id to frame_id & BCBs
        }
    }
}  

// void NewPage FixNewPage();
void BMgr::FixNewPage() {
    // dsMgr -> WritePage();     

}
int BMgr::UnfixPage(int page_id) {
    if(ptob[page_id] != nullptr) {
        BCB *b = findByPageID(b, page_id);
        b->count -= 1;
    }

}
int BMgr::NumFreeFrames() {
    return freeList.size();
}

// Internal Functions
int BMgr::SelectVictim() {
}

int BMgr::Hash(int page_id) {
    return page_id % BUFFER_SIZE;
}

void BMgr::RemoveBCB(BCB * ptr, int page_id) {
    // TODO: write the page into disk
    // TODO: simply remove the BCB
    BCB* bcb = findByPageID(ptr, page_id);
    if(bcb != nullptr) {
        // TODO: 清除hashtable信息
        // TODO: 更新BCB信息
        int frame_id = pages[page_id];
        pages[page_id] = -1;
        delete(bcb);
    }
    return;
}

void BMgr::RemoveLRUEle(int frid) { // f_rid?
    replacer -> remove(frid);
}

void BMgr::SetDirty(int frame_id) {
    BCB* bcb = buffer[frame_id];
    bcb -> dirty = 1;
    return;
}

void BMgr::UnsetDirty(int frame_id) {
    BCB* bcb = buffer[frame_id];
    bcb->dirty = 0;
    return;
}   

void BMgr::WriteDirtys() { // 将buffer中的frame的内容写入disk(page)
    for(int i = 0; i < DEFBUF_SIZE; i++) { 
        BCB * bcb = buffer[i];
        if(bcb->dirty == 1) {
            dsMgr -> WritePage(bcb->frame_id, frames[bcb->frame_id]);
            bcb->dirty = 0;
        }
    }
}

void BMgr::PrintFrame(int frame_id) { // prints out the contents of the frame described by the frame_id.
    printf("print frame: %s\n", frames[frame_id].field);
}
    