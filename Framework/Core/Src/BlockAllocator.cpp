#include "Precompiled.h"
#include "BlockAllocator.h"

#include "DebugUtil.h"

NEngine::Core::BlockAllocator::BlockAllocator(const char* name, size_t blockSize, size_t capacity)
	: mName(name), mBlockSize(blockSize), mCapacity(capacity), mFreeBlocks(capacity)
{
	ASSERT(blockSize > 0, "BlockAllocator: invalid blockSize");
	ASSERT(capacity > 0, "BlockAllocator: invalid capacity");

	mData = std::malloc(blockSize * capacity);
	
	for (size_t i = 0; i < capacity; ++i)
	{
		mFreeBlocks[i] = static_cast<uint8_t*>(mData) + (i * mBlockSize);
	}

	LOG("%s allocated %zu blocks, block size: %zu", mName.c_str(), capacity, blockSize);
}

NEngine::Core::BlockAllocator::~BlockAllocator()
{
	ASSERT(mBlocksAllocatedTotal == mBlocksFreed, "BlockAllocator: not all blocks freed");

	std::free(mData);
	LOG("%s destructed, Allocated: %zu, Freed: %zu, Highest: %zu", mName.c_str(), mBlocksAllocatedTotal, mBlocksFreed, mBlocksHighest);
}

void* NEngine::Core::BlockAllocator::Allocate()
{
	if (mFreeBlocks.empty())
	{
		LOG("%s no free blocks available", mName.c_str());
		return nullptr;
	}
	
	void* freeBlock = mFreeBlocks.back();
	mFreeBlocks.pop_back();

	++mBlocksAllocatedCurrent;
	++mBlocksAllocatedTotal;
	mBlocksHighest = std::max(mBlocksHighest, mBlocksAllocatedCurrent);

	LOG("%s allocated blocks at %p, Allocated: %zu, HighestCount: %zu", mName.c_str(), freeBlock, mBlocksAllocatedCurrent, mBlocksHighest);

	return freeBlock;
}

void NEngine::Core::BlockAllocator::Free(void* ptr)
{
	if (ptr == nullptr)
		return;

	const auto start = static_cast<uint8_t*>(mData);
	const auto end = static_cast<uint8_t*>(mData) + (mBlockSize * mCapacity);
	const auto current = static_cast<uint8_t*>(ptr);
	const auto diff = current - start;

	ASSERT(current >= start && current < end && diff % mBlockSize == 0, "BlockAllocator: Invalid Address Being Freed");
	LOG("%s freed %p", mName.c_str(), ptr);

	--mBlocksAllocatedCurrent;
	++mBlocksFreed;
	mFreeBlocks.emplace_back(ptr);
	ptr = nullptr;

}
