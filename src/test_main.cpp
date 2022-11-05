#include <iostream>
#include <string>

#include "lru_replacer.h"
#include "buffer_manager.h"
#include "storage.h"
#include "storage_manager.h"

const std::string filename = "../data/data.dbf";
const std::string data_in = "../data/data-5w-50w-zipf.txt";

DSMgr * dsMgr;
LRUReplacer * replacer;
BMgr * bMgr;

void init() {
    dsMgr = new DSMgr(filename);
    replacer = new LRUReplacer(DEFBUF_SIZE);
    bMgr = new BMgr(dsMgr, replacer);
    for(int i=0; i<TOTAL_PAGE_NUM; i++) {
        bMgr -> FixNewPage();
    }
    std::cout<<"Init() done."<<std::endl;
}

void test() {
    FILE *fp = fopen(data_in.c_str(), "r+");
    while(true) {
        char data[12] = {};
        fgets(data, sizeof(data), fp);
        if (feof(fp))
            break;
        data[1] = '\0';
        int op = std::stoi(data);
        int page_id = std::stoi(data + 2);
        if(op == 0) {
            bMgr -> FixPage(page_id, 0);
        } else if(op == 1) {
            bMgr -> FixPage(page_id, 1);
        }
    }
    fclose(fp);
    std::cout<< "page num: "<< dsMgr -> GetNumPages() << std::endl;   
    std::cout<< "hit num: "<< bMgr -> GetHitCnt() << std::endl;
    std::cout<< "IO  num: "<< dsMgr -> GetIOCnt() << std::endl;
}

int main() {
    init();
    test();
}