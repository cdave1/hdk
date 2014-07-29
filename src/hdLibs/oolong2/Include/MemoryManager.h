#ifndef MEMORY_H_
#define MEMORY_H_

#ifdef __APPLE__
#include <TargetConditionals.h>

#include "../Utility/MemoryManager/MemoryTemplates.h"

#if TARGET_OS_IPHONE && (!_NOMEMORYMANAGER)
#include "../Utility/MemoryManager/mmgr.h"
#else
#include "../Utility/MemoryManager/nommgr.h"
#endif



#endif
#endif