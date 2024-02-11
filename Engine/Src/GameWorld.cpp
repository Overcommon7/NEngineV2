#include "Precompiled.h"
#include "GameWorld.h"

#include "GameObjectFactory.h"
#include "Components/Transform.h"
#include "Components/RigidbodyComponent.h"

#include "Services/UpdateService.h"
#include "Services/CameraService.h"
#include "Services/RenderService.h"
#include "Services/PhysicsService.h"

namespace
{
	NEngine::CustomService TryMakeService;
}

void NEngine::GameWorld::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "World Already Initialized");

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
			slot.gameObject.reset();
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
	for (auto& slot : mSlots)
	{
		if (slot.gameObject)
			slot.gameObject->DebugUI();
	}

	for (auto& service : mServices)
		service->DebugUI();
}

NEngine::GameObject* NEngine::GameWorld::CreateGameObject(const std::filesystem::path& templateFile)
{
	ASSERT(mInitialized, "");

	if (mFreeSlots.empty())
	{
		return nullptr;
	}

	const uint32_t freeSlot = mFreeSlots.back();
	mFreeSlots.pop_back();

	Slot& slot = mSlots[freeSlot];
	std::unique_ptr<GameObject>& newObject = slot.gameObject;
	newObject = std::make_unique<GameObject>();

	if (!templateFile.empty() && std::filesystem::exists(templateFile))
		GameObjectFactory::Make(templateFile, *newObject);
	else newObject->AddComponent<Transform>();


	newObject->mWorld = this;
	newObject->mHandle.mIndex = freeSlot;
	newObject->mHandle.mGeneration = slot.generation;
	newObject->Initialize();
	return newObject.get();
}

NEngine::GameObject* NEngine::GameWorld::GetGameObject(const GameObjectHandle& handle)
{
	if (!IsValid(handle))
		return nullptr;

	return mSlots[handle.mIndex].gameObject.get();
}

void NEngine::GameWorld::DestroyGameObject(const GameObjectHandle& handle)
{
	if (!IsValid(handle))
		return;

	auto& slot = mSlots[handle.mIndex];
	++slot.generation;

	mToBeDestroyed.push_back(handle.mIndex);
}

void NEngine::GameWorld::LoadLevel(const std::filesystem::path& levelFile)
{
	FILE* file = nullptr;

	

	auto err = fopen_s(&file, levelFile.string().c_str(), "r");
	ASSERT(err == 0 && file != nullptr, "GameWorld: failed to load level %s", levelFile.string().c_str());
	
	string readBuffer;
	readBuffer.resize(USHRT_MAX);

	rapidjson::FileReadStream readStream(file, readBuffer.data(), readBuffer.size());
	fclose(file);

	rapidjson::Document doc;
	doc.ParseStream(readStream);

	auto services = doc["Services"].GetObj();
	for (auto& service : services)
	{
		string serviceName(service.name.GetString());
		if (TryMakeService(serviceName, service.value, *this))
		{

		}
		else if (serviceName == "Camera")
		{
			CameraService* cameraService = AddService<CameraService>();
			cameraService->Deserialize(service.value);
		}
		else if (serviceName == "Update")
		{
			UpdateService* updateService = AddService<UpdateService>();
			updateService->Deserialize(service.value);
		}
		else if (serviceName == "Render")
		{
			RenderService* renderService = AddService<RenderService>();
			renderService->Deserialize(service.value);
		}
		else if (serviceName == "Physics")
		{
			PhysicsService* physicsService = AddService<PhysicsService>();
			physicsService->Deserialize(service.value);
		}
	}

	uint32_t capacity = static_cast<uint32_t>(doc["Capacity"].GetInt());
	Initialize(capacity);

	auto gameObjects = doc["GameObjects"].GetObj();
	for (auto& gameObject : gameObjects)
	{
		string templateFile(gameObject.value["Template"].GetString());
		GameObject* obj = CreateGameObject(templateFile);

		if (!obj) continue;
		obj->SetName(gameObject.name.GetString());
		
		if (gameObject.value.HasMember("Position"))
		{
			auto transform = obj->GetComponent<Transform>();
			auto rigidbody = obj->GetComponent<RigidbodyComponent>();
			const auto& position = gameObject.value["Position"].GetArray();
			for (int i = 0; i < 3; ++i)
			{
				transform->position.v[i] = position[i].GetFloat();	
			}
			if (rigidbody)
				rigidbody->SetPosition(transform->position);
		}
	}
}

void NEngine::GameWorld::SetCustomService(CustomService customService)
{
	TryMakeService = customService;
}

bool NEngine::GameWorld::IsValid(const GameObjectHandle& handle)
{
	if (handle.mIndex < 0 || handle.mIndex >= mSlots.size())
		return false;

	if (mSlots[handle.mIndex].generation != handle.mGeneration)
		return false;
	
	return true;
}

void NEngine::GameWorld::ProcessDestroyList()
{
	for (uint32_t index : mToBeDestroyed)
	{
		auto& slot = mSlots[index];
		auto gameObject = slot.gameObject.get();

		ASSERT(!IsValid(gameObject->mHandle), "GameWorld: object is still alive");

		gameObject->Terminate();
		slot.gameObject.reset();
		mFreeSlots.push_back(index);
	}

	mToBeDestroyed.clear();
}
