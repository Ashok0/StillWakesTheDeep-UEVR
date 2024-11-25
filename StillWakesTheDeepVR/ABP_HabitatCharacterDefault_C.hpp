#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class ABP_HabitatCharacterDefault_C : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"AnimBlueprintGeneratedClass /Game/Habitat/Pawns/Player/Rig/ABP_HabitatCharacterDefault.ABP_HabitatCharacterDefault_C");
		}
		return result;
	}

	static ABP_HabitatCharacterDefault_C* get_instance()
	{
		auto klass = ABP_HabitatCharacterDefault_C::static_class();
		ABP_HabitatCharacterDefault_C* Object = nullptr;

		if (klass) {

			std::vector<ABP_HabitatCharacterDefault_C*> List = klass->get_objects_matching<ABP_HabitatCharacterDefault_C>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("ABP_HabitatCharacterDefault_C: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());				

				return Object;
			}

			return Object;
		}
		return nullptr;
	}

	bool get_IsInteracting()
	{
		return get_bool_property(L"IsInteracting");
	}

	bool get_IsAnimationControllingCamera()
	{
		return get_bool_property(L"IsAnimationControllingCamera");
	}

	bool get_IsBalancing()
	{
		return get_bool_property(L"IsBalancing");
	}

	bool get_IsCrouching()
	{
		return get_bool_property(L"IsCrouching");
	}

	bool get_IsCrawling()
	{
		return get_bool_property(L"IsCrawling");
	}
};