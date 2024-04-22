#ifdef _DEBUG
#include <crtdbg.h>
#define  malloc( s ) _malloc_dbg( s, _CLIENT_BLOCK, __FILE__, __LINE__ )
#define  realloc( s, d ) _realloc_dbg( s, d, _CLIENT_BLOCK, __FILE__, __LINE__ )
#define  free( s ) _free_dbg( s, _CLIENT_BLOCK )
#endif

