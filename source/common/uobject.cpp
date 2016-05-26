/*
******************************************************************************
*
*   Copyright (C) 2016 and later: Unicode, Inc. and others.
*   License & terms of use: http://www.unicode.org/copyright.html
*
******************************************************************************
*   file name:  uobject.h
*   encoding:   US-ASCII
*   tab size:   8 (not used)
*   indentation:4
*
*   created on: 2002jun26
*   created by: Markus W. Scherer
*/

#include "unicode/uobject.h"
#include "cmemory.h"

U_NAMESPACE_BEGIN

#if U_OVERRIDE_CXX_ALLOCATION

/*
 * Default implementation of UMemory::new/delete
 * using uprv_malloc() and uprv_free().
 *
 * For testing, this is used together with a list of imported symbols to verify
 * that ICU is not using the global ::new and ::delete operators.
 *
 * These operators can be implemented like this or any other appropriate way
 * when customizing ICU for certain environments.
 * Whenever ICU is customized in binary incompatible ways please be sure
 * to use library name suffixes to distinguish such libraries from
 * the standard build.
 *
 * Instead of just modifying these C++ new/delete operators, it is usually best
 * to modify the uprv_malloc()/uprv_free()/uprv_realloc() functions in cmemory.c.
 *
 * Memory test on Windows/MSVC 6:
 * The global operators new and delete look as follows:
 *   04F 00000000 UNDEF  notype ()    External     | ??2@YAPAXI@Z (void * __cdecl operator new(unsigned int))
 *   03F 00000000 UNDEF  notype ()    External     | ??3@YAXPAX@Z (void __cdecl operator delete(void *))
 *
 * These lines are from output generated by the MSVC 6 tool dumpbin with
 * dumpbin /symbols *.obj
 *
 * ??2@YAPAXI@Z and ??3@YAXPAX@Z are the linker symbols in the .obj
 * files and are imported from msvcrtd.dll (in a debug build).
 *
 * Make sure that with the UMemory operators new and delete defined these two symbols
 * do not appear in the dumpbin /symbols output for the ICU libraries!
 *
 * If such a symbol appears in the output then look in the preceding lines in the output
 * for which file and function calls the global new or delete operator,
 * and replace with uprv_malloc/uprv_free.
 */

void * U_EXPORT2 UMemory::operator new(size_t size) U_NO_THROW {
    return uprv_malloc(size);
}

void U_EXPORT2 UMemory::operator delete(void *p) U_NO_THROW {
    if(p!=NULL) {
        uprv_free(p);
    }
}

void * U_EXPORT2 UMemory::operator new[](size_t size) U_NO_THROW {
    return uprv_malloc(size);
}

void U_EXPORT2 UMemory::operator delete[](void *p) U_NO_THROW {
    if(p!=NULL) {
        uprv_free(p);
    }
}

#if U_HAVE_DEBUG_LOCATION_NEW
void * U_EXPORT2 UMemory::operator new(size_t size, const char* /*file*/, int /*line*/) U_NO_THROW {
    return UMemory::operator new(size);
}

void U_EXPORT2 UMemory::operator delete(void* p, const char* /*file*/, int /*line*/) U_NO_THROW {
    UMemory::operator delete(p);
}
#endif /* U_HAVE_DEBUG_LOCATION_NEW */


#endif

UObject::~UObject() {}

UClassID UObject::getDynamicClassID() const { return NULL; }

U_NAMESPACE_END

U_NAMESPACE_USE

U_CAPI void U_EXPORT2
uprv_deleteUObject(void *obj) {
    delete static_cast<UObject *>(obj);
}
