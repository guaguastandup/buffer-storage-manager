#include "include/storage/storage.h"
#include <string>
#include <fstream>

using namespace std;

class DSMgr { // disk storage manager
public:
       DSMgr();
       int OpenFile(string filename);
       int CloseFile();

       bFrame ReadPage(int page_id);
       int WritePage(int frame_id, bFrame frm);

       int Seek(int offset, int pos);
       FILE * GetFile();
       
       void IncNumPages();
       int GetNumPages();

       void SetUse(int index, int use_bit);
       int GetUse(int index);
private:
       std::fstream dbIO;
	FILE *currFile;

	int numPages;
	int pages[MAXPAGES];
};