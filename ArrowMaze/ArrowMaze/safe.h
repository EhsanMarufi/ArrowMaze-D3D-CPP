// In the name of God
// Ehsan Marufi Azar, 2012 (c)
// v1.1 Jan 2013

#pragma once;
#ifndef SAFE_H
#define SAFE_H


#ifndef SAFE_DELETE
#define SAFE_DELETE(p){if(p){delete(p);(p)=NULL;}}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p){if(p){delete[](p);(p)=NULL;}}
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p){if(p){(p)->Release();(p)=NULL;}}
#endif


#endif // SAFE_H