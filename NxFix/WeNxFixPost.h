#ifdef WINDENGINE_CRTDBG_MAP_ALLOC
#undef WINDENGINE_CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC

#pragma pop_macro("malloc")
#pragma pop_macro("realloc")
#pragma pop_macro("free")
#pragma pop_macro("_strdup")
#pragma pop_macro("strdup")
#pragma pop_macro("_wcsdup")
#pragma pop_macro("wcsdup")
#pragma pop_macro("_wgetcwd")
#pragma pop_macro("_wgetdcwd")
#pragma pop_macro("_wgetdcwd_nolock")

#endif
