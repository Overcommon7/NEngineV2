#pragma once

// including all our external libs
#include <Core/Inc/Core.h>
#include <Input/Inc/Input.h>
#include <Math/Inc/NMath.h>
#include <Graphics/Inc/Colors.h>
#include <Graphics/Inc/Graphics.h>
#include <Physics/Inc/Physics.h>
#include <../Audio/Inc/Audio.h>

//rapid json
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

#define CMP_VALUE rapidjson::Value componentValue(rapidjson::kObjectType)
