
#include <QRect>

typedef void (__cdecl *MouseCallback )(int event, int x, int y, int flags, void* param);

#define DELETEPTR(ptr)  if(ptr) {delete ptr; ptr = nullptr;}
#define SAFEDELETE(pData) { try { delete pData; } catch (...) { ASSERT(FALSE); } pData=NULL; }
