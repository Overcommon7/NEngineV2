#pragma once

namespace NEngine::Core
{
	class BlockAllocator
	{
	public:
		BlockAllocator(const char* name, size_t blockSize, size_t capacity);
		virtual ~BlockAllocator();

		void* Allocate();
		void Free(void* ptr);
	private:
		string mName;
		vector<void*> mFreeBlocks;

		void* mData = nullptr;
		size_t mBlockSize = 0;
		size_t mCapacity;
		size_t mBlocksAllocatedCurrent = 0;
		size_t mBlocksAllocatedTotal = 0;
		size_t mBlocksFreed = 0;
		size_t mBlocksHighest = 0;
	public:
		
		BlockAllocator(const BlockAllocator& b) = delete;
		BlockAllocator(const BlockAllocator&& b) = delete;
		BlockAllocator& operator=(const BlockAllocator& b) = delete;
		BlockAllocator& operator=(const BlockAllocator&& b) = delete;

	};
}
