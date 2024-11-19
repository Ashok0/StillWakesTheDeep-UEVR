#pragma once

#include "uevr/API.hpp"

using namespace uevr;
bool IsMouseCursor = false;

class PlayerController : public API::UObject
{
public:
	using API::UObject::get_full_name;

	static API::UClass* static_class()
	{
		static API::UClass* result = nullptr;
		if (!result) {
			result = API::get()->find_uobject<API::UClass>(L"Class /Script/Engine.PlayerController");
		}
		return result;
	}

	static PlayerController* get_instance()
	{
		auto klass = PlayerController::static_class();
		PlayerController* Object = nullptr;

		if (klass) {

			std::vector<PlayerController*> List = klass->get_objects_matching<PlayerController>();

			for (size_t i = 0; i < List.size(); i++) {
				Object = List[i];

				std::wstring ObjName = Object->get_full_name();
				// API::get()->log_info("PlayerController: Object %d of %d, Object name: %ls", i, List.size(), ObjName.c_str());				

				IsMouseCursor = Object->get_bShowMouseCursor();

				if (IsMouseCursor == true)
				{
					return Object;
				}
			}

			return Object;
		}
		return nullptr;
	}

	bool get_bShowMouseCursor()
	{
		return get_bool_property(L"bShowMouseCursor");
	}	
};