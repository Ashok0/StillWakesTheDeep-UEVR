#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class HabitatCharacterBase : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"Class /Script/Habitat.HabitatCharacterBase");
		}
		return result;
	}

	static HabitatCharacterBase* get_instance()
	{
		auto klass = HabitatCharacterBase::static_class();
		HabitatCharacterBase* Object = nullptr;

		if (klass) {

			std::vector<HabitatCharacterBase*> List = klass->get_objects_matching<HabitatCharacterBase>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("HabitatCharacterBase: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());				

				bool IsInCutscene = Object->get_bIsInCutscene();
				// API::get()->log_info("IsInCutscene = %d", IsInCutscene);

				if (IsInCutscene == true)
				{
					return Object;
				}
			}

			return Object;
		}
		return nullptr;
	}

	bool get_bIsInCutscene()
	{
		return get_bool_property(L"bIsInCutscene");
	}	
};