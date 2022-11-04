#include "include/storage/storage.h"
#include <string>
#include <fstream>

class DSMgr { // disk storage manager
public:
       DSMgr();
       DSMgr::DSMgr(std::string filename = "./data.dbf");

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
private:
       // std::fstream fileIO;
	FILE *currFile;
       
	int numPages;

       long long io_total;
       
};