#include "storage.h"
#include "storage_manager.h"

#include <iostream>

DSMgr::DSMgr() {
  io_total = 0;
  currFile = nullptr;
  numPages = 0;
}

DSMgr::DSMgr(std::string filename) {
  io_total = 0;
  currFile = nullptr;
  numPages = 0;
  OpenFile(filename);
}

DSMgr::~DSMgr() {
  CloseFile();
}

// file operation
int DSMgr::OpenFile(std::string filename) {
  currFile = fopen(filename.c_str(), "wb+");
  if(currFile == NULL){
    std::cout<<"no file: "<<filename<<std::endl;
    return -1;
  }
  return 0;
}
int DSMgr::CloseFile() {
  return fclose(currFile);
}

// page operation
// remember to count the IO operation
bFrame DSMgr::ReadPage(int page_id) { //  将page读到frame中
  // read the content in page
  int offset = page_id * PAGE_SIZE;
  fseek(currFile, offset, SEEK_SET); // get the offset
  bFrame frame;
  size_t sz = fread(frame.field, 1, PAGE_SIZE, currFile);
  if(sz < PAGE_SIZE) {
    memset(frame.field + sz, 0, PAGE_SIZE - sz);
  }
  IncIO();
  return frame;
}

// issue: the dsmgr doesn't know the corresbonding frame_id of a page
// so maybe we should put page_id rather than frame_id
// return: how many bytes were written
int DSMgr::WritePage(int page_id, bFrame frm) {  // write frame to page: cost diskIO
  // std::cout << "ok0 " << std::endl;
  int offset = page_id * PAGE_SIZE;
  // std::cout << "ok1 " << std::endl;
  fseek(currFile, offset, SEEK_SET);
  // std::cout << "ok2 " << std::endl;
  fwrite(frm.field, 1, PAGE_SIZE, currFile);
  // std::cout << "ok3 " << std::endl;
  fflush(currFile); // to keep disk file in sync
  // std::cout << "ok4 " << std::endl;
  IncIO();
  // std::cout << "ok5 " << std::endl;
  return strlen(frm.field);
  // return PAGE_SIZE;
}

int DSMgr::Seek(int offset, int pos) {
  int ret = fseek(currFile, offset, pos);
  return ret;
}

FILE* DSMgr::GetFile() {
  return currFile;
}

void DSMgr::IncNumPages() { numPages += 1; }
int DSMgr::GetNumPages() { return numPages; }

// use operation
void DSMgr::SetUse(int page_id, int use_bit) {
  page_usebit[page_id] = use_bit;
}

int DSMgr::GetUse(int page_id) {
  return page_usebit[page_id];
}

void DSMgr::IncIO() {
  io_total += 1;
}