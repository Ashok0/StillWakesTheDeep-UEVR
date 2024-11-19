#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class HabitatSavedSettings : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"Class /Script/Habitat.HabitatSavedSettings");
		}
		return result;
	}

	static HabitatSavedSettings* get_instance()
	{
		auto klass = HabitatSavedSettings::static_class();
		HabitatSavedSettings* Object = nullptr;

		if (klass) {

			std::vector<HabitatSavedSettings*> List = klass->get_objects_matching<HabitatSavedSettings>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("HabitatSavedSettings: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());

				if (ObjName.find(L"HabitatSavedSettings") != std::wstring::npos)
				{
					// API::get()->log_info("HabitatSavedSettings found");
					Object->SetHeadBobAmount(0.0);
				}
				else
				{
					// API::get()->log_info("HabitatSavedSettings not found");
				}
			}
		}
		return nullptr;
	}

	void SetHeadBobAmount(float Value)
	{
		static const auto func = HabitatSavedSettings::static_class()->find_function(L"SetHeadBobAmount");
		if (!func) {
			// API::get()->log_info("SetHeadBobAmount not found");
			return;
		}
		else
		{
			// API::get()->log_info("SetHeadBobAmount found");
		}

		struct
		{
			float Value;
		} params{ .Value = Value };

		process_event(func, &params);
	}	
};
