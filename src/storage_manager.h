#ifndef STORAGE_MANAGER
#define STORAGE_MANAGER
#include "storage.h"
#include <string>
#include <fstream>

class DSMgr { // disk storage manager
public:
       DSMgr();
       DSMgr(std::string filename = "./data.dbf");
       ~DSMgr();

       int OpenFile(std::string filename);
       int CloseFile();

       bFrame ReadPage(int page_id);
       int WritePage(int frame_id, bFrame frm);

       int Seek(int offset, int pos);
       FILE * GetFile();

       void IncNumPages();
       int GetNumPages();

       void SetUse(int index, int use_bit);
       int GetUse(int index);

       void IncIO();
       int GetIOCnt() { return io_total; }
private:
       // std::fstream fileIO;
	FILE *currFile;
	int numPages = 0;
       long long io_total = 0;
       std::unordered_map<int, int> page_usebit;

       
};

#endif