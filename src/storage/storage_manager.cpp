#include "include/storage/storage.h"
#include "include/storage/storage_manager.h"

DSMgr::DSMgr() {

}
// file operation
int DSMgr::OpenFile(string filename) {

}
int DSMgr::CloseFile() {

}

// page operation
bFrame DSMgr::ReadPage(int page_id) {

}

int DSMgr::WritePage(int frame_id, bFrame frm) {

}
int DSMgr::Seek(int offset, int pos) {

}

FILE* DSMgr::GetFile() {
//    FILE* fp;
//    fp = fopen("../data/data.abf", "rw");
//    return fp;
  return currFile;
}

void DSMgr::IncNumPages() {

}

int DSMgr::GetNumPages() {
  return numPages;
}

// use operation

void DSMgr::SetUse(int index, int use_bit) {

}

int DSMgr::GetUse(int index) {

}
