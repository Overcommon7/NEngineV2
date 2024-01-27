#include "Precompiled.h"
#include "GameWorld.h"

#include "GameObjectFactory.h"

void NEngine::GameWorld::Intialize(uint32_t capacity)
{
	ASSERT(mInitialized, "World Already Initialized");

	for (auto& service : mServices)
		service->Initialize();

	mSlots.resize(capacity);
	mFreeSlots.resize(capacity);
	std::iota(mFreeSlots.begin(), mFreeSlots.end(), 0);

	mInitialized = true;
}

void NEngine::GameWorld::Terminate()
{
	ASSERT(!mIsUpdating, "Can not terminate while updating");
	for (auto& slot : mSlots)
	{
		if (slot.gameObject != nullptr)
		{
			slot.gameObject->Terminate();
		}
	}

	for (auto& service : mServices)
		service->Terminate();

	mInitialized = false;
}

void NEngine::GameWorld::Update(float deltaTime)
{
	mIsUpdating = true;

	for (auto& service : mServices)
		service->Update(deltaTime);

	mIsUpdating = false;
}

void NEngine::GameWorld::Render()
{
	for (auto& service : mServices)
		service->Render();
}

void NEngine::GameWorld::DebugUI()
{
	for (auto& service : mServices)
		service->DebugUI();
}

NEngine::GameObject* NEngine::GameWorld::CreateGameObject(const std::filesystem::path& templateFile)
{
	ASSERT(!mInitialized, "");

	if (mFreeSlots.empty())
	{
		return;
	}

	const uint32_t freeSlot = mFreeSlots.back();
	mFreeSlots.pop_back();

	Slot& slot = mSlots[freeSlot];
	std::unique_ptr<GameObject>& newObject = slot.gameObject;
	newObject = std::make_unique<GameObject>();

	GameObjectFactory::Make(templateFile, *newObject);

	newObject->mWorld = this;
	newObject->mHandle.mIndex = freeSlot;
	newObject->mHandle.mGeneration = slot.generation;
	newObject->Initialize();
	return newObject.get();
}

NEngine::GameObject* NEngine::GameWorld::GetGameObject(const GameObjectHandle& handle)
{
	return nullptr;
}

void NEngine::GameWorld::DestroyGameObject(const GameObjectHandle& handle)
{
}

bool NEngine::GameWorld::IsValid(const GameObjectHandle& handle)
{
	return false;
}

void NEngine::GameWorld::ProcessDestroyList()
{
}
