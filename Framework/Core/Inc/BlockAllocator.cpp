#include "Precompiled.h"
#include "BlockAllocator.h"

NEngine::Core::BlockAllocator::BlockAllocator(const char* name, size_t blockSize, size_t capacity)
{
}

NEngine::Core::BlockAllocator::~BlockAllocator()
{
}

void* NEngine::Core::BlockAllocator::Allocate()
{
    return nullptr;
}

void* NEngine::Core::BlockAllocator::Free(void* ptr)
{
    return nullptr;
}
