#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class HabitatCharacter : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"Class /Script/Habitat.HabitatCharacter");
		}
		return result;
	}

	static HabitatCharacter* get_instance()
	{
		auto klass = HabitatCharacter::static_class();
		HabitatCharacter* Object = nullptr;

		if (klass) {

			std::vector<HabitatCharacter*> List = klass->get_objects_matching<HabitatCharacter>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("HabitatCharacter: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());				

				bool IsRunningCinematic = Object->get_bIsRunningCinematic();
				// API::get()->log_info("IsRunningCinematic = %d", IsRunningCinematic);

				if (IsRunningCinematic == true)
				{
					return Object;
				}
			}

			return Object;
		}
		return nullptr;
	}

	bool get_bIsRunningCinematic()
	{
		return get_bool_property(L"bIsRunningCinematic");
	}
};