#include "storage.h"
#include "buffer_manager.h"
#include <iostream>
BMgr::BMgr(DSMgr *dsMgr, LRUReplacer* replacer) {
    this->dsMgr = dsMgr;
    this->replacer = replacer;
    freeList.clear();
    for(int i=0; i<DEFBUF_SIZE; i++) {
        this -> freeList.emplace_back(i);
    }
    memset(pages, -1, sizeof(pages));
}

BMgr::~BMgr() {
    delete replacer;
    for(int i=0; i<DEFBUF_SIZE; i++) {
        if(buffer[i] != nullptr) {
            delete(buffer[i]);
        }
    }
}

// Interface functions
int BMgr::FixPage(int page_id, int prot) { // 更新page的BCB
    int frame_id = pages[page_id];

    // 1. hit in buffer
    if(frame_id != -1) {
        IncHitCnt();
        frame_id = pages[page_id];
        replacer->Unpin(frame_id); // update the order in LRUReplacer
        if(prot == 1) { // write operation
            SetDirty(frame_id); 
        }
        return frame_id;
    }
    // 2. not in buffer
    // 2.1 select victim
    int victim_id = SelectVictim();
    assert(victim_id != -1);
    // 2.2 get the victim
    BCB * victim_bcb = buffer[victim_id];
    // 2.2.1 if the victim is got by LRUReplacer
    // 2.2.2 else: it is from free page
    if(victim_bcb != nullptr) {
        victim_bcb->frame_id = -1;
        pages[victim_bcb->page_id] = -1; // page_id to frame_id
        if(victim_bcb -> dirty) {
            dsMgr -> WritePage(victim_bcb->page_id, frames[victim_id]);
            victim_bcb -> dirty = 0;
        }
    }
    frames[victim_id] = dsMgr -> ReadPage(page_id); // load the target page in bFrame
    pages[page_id] = victim_id;
    BCB * bcb = GetBCB(page_id);
    bcb -> frame_id = victim_id;
    buffer[victim_id] = bcb; // load in buffer
    if(prot == 1) { 
        // write something
        bcb -> dirty = 1;
    }
    replacer->Unpin(victim_id);
    return victim_id;
}  

void BMgr::FixNewPage() {
    dsMgr -> IncNumPages();
    int page_id = dsMgr -> GetNumPages();
    bFrame tmpframe;
    memset(tmpframe.field, 0, sizeof(tmpframe.field));
    for(int i=0;i < FRAME_SIZE; i++) {
        tmpframe.field[i] = (char)('0' + (i%10));
    }
    // std::cout << "ok: "<< tmpframe.field << std::endl;
    dsMgr -> WritePage(page_id, tmpframe);
    FixPage(page_id, 0);
}

int BMgr::UnfixPage(int page_id) { // 对该页的调用-1
    // the page is in buffer
    int frame_id = pages[page_id];
    BCB* bcb = buffer[frame_id];
    assert(bcb != nullptr);
    assert(bcb -> count > 0);
    bcb -> count -= 1; 
    if(bcb -> count == 0) {
        replacer -> Unpin(frame_id); // 表示这一页可以被作为victim了, 并且这一页是在buffer中的
        // freeList.emplace_back(frame_id);
    }
    return 1;
}

int BMgr::NumFreeFrames() {
    return freeList.size();
}

// Internal Functions
int BMgr::SelectVictim() {
    if(freeList.size() > 0) { 
        int ret = freeList.back();
        freeList.pop_back();
        // std::cout<< "from freeList : " << ret << std::endl;
        return ret;
    }
    if(replacer -> Size() == 0) {
        return -1;
    }
    int frame_id;
    replacer -> Victim(&frame_id);
    // std::cout<< "from replacer : " << frame_id << std::endl;
    return frame_id;
}

int BMgr::Hash(int page_id) {
    return page_id % BUFFER_SIZE;
}

// 如果这一页存在, 则存在一个BCB
// 在buffer中只有一个BCB
// BCB用于查看page的信息
void BMgr::RemoveBCB(BCB * ptr, int page_id) {  // no need
    // ptr = ptof[H(page_id)]
    assert(ptr->count == 0);
    assert(ptr->dirty == 0);
    BCB * pre = ptr;
    ptr = ptr -> next;
    while(ptr != nullptr) {
        if(ptr -> page_id == page_id) {
            pre->next = ptr -> next;
            pages[page_id] = -1;
            if(ptr -> frame_id != -1) {
                int frame_id = ptr -> frame_id;
                // 说明这一页在buffer中
                // 这种情况貌似不存在
                buffer[frame_id] = nullptr;
                // 此时frames还没清空, 但是不重要
            }
            delete(ptr);
        }
    }
}

BCB * BMgr::GetBCB(int page_id) {
    int h = Hash(page_id);
    BCB * bcb = ptob[h];
    if(bcb == nullptr) {
        ptob[h] = new BCB(page_id);
        return ptob[h];
    }
    while(bcb != nullptr) {
        if(bcb -> page_id == page_id) {
            return bcb;
        }
        if(bcb != nullptr && bcb -> next == nullptr) {
            bcb -> next = new BCB(page_id);
            return bcb -> next;
        }
        bcb = bcb -> next;
    }
    return nullptr;
}

void BMgr::RemoveLRUEle(int frid) { }

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
    