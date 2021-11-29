#ifdef DEBUG
#undef DEBUG
#define WINDENGINE_CEGUI_DEBUG
#endif

#ifdef _DEBUG
#undef _DEBUG
#define WINDENGINE_CEGUI__DEBUG
#endif

#include <CEGUI.h>

#ifdef WINDENGINE_CEGUI_DEBUG
#undef WINDENGINE_CEGUI_DEBUG
#define DEBUG
#endif

#ifdef WINDENGINE_CEGUI__DEBUG
#undef WINDENGINE_CEGUI__DEBUG
#define _DEBUG
#endif
