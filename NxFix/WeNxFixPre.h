#ifdef _CRTDBG_MAP_ALLOC
#undef _CRTDBG_MAP_ALLOC
#define WINDENGINE_CRTDBG_MAP_ALLOC

#pragma push_macro("malloc")
#undef malloc
#pragma push_macro("realloc")
#undef realloc
#pragma push_macro("free")
#undef free
#pragma push_macro("_strdup")
#undef _strdup
#pragma push_macro("strdup")
#undef strdup
#pragma push_macro("_wcsdup")
#undef _wcsdup
#pragma push_macro("wcsdup")
#undef wcsdup
#pragma push_macro("_wgetcwd")
#undef _wgetcwd
#pragma push_macro("_wgetdcwd")
#undef _wgetdcwd
#pragma push_macro("_wgetdcwd_nolock")
#undef _wgetdcwd_nolock

#endif
