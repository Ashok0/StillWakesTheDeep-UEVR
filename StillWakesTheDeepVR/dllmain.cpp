#define  _CRT_SECURE_NO_WARNINGS 1
#define  _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <memory>
#include <ctime>
#include <iostream>
#include <fstream>

#include "uevr/Plugin.hpp"
#include "cxxtimer.hpp"
#include "ABP_HabitatCharacterDefault_C.hpp"
#include "HabitatCharacter.hpp"
#include "HabitatCharacterBase.hpp"
#include "HabitatSavedSettings.hpp"
#include "PlayerController.hpp"

void reset_height_balancing();
void reset_height();

// Instantiate cxxtimer::Timer object
cxxtimer::Timer timer;

bool IsPawn = false;
bool IsExamining = false;
bool IsNoPushback = false;
bool IsInteracting = false;
bool IsCutscenePrev = false;
bool IsCutsceneCurrent = false;
bool IsBalancingPrev = false;
bool IsBalancingCurrent = false;
bool IsMouseCursorPrev = false;
bool IsMouseCursorCurrent = false;
bool IsAnimationControllingCameraPrev = false;
bool IsAnimationControllingCameraCurrent = false;

typedef struct _TIMER_STRUCT
{
    bool* XPressed;
    std::time_t* TimeStamp;
}   TIMER_STRUCT;

void DebugPrint(char* Format, ...);
using namespace uevr;

#define PLUGIN_LOG_ONCE(...) \
    static bool _logged_ = false; \
    if (!_logged_) { \
        _logged_ = true; \
        API::get()->log_info(__VA_ARGS__); \
    }


class StillWakesTheDeepPlugin : public uevr::Plugin {
public:
    StillWakesTheDeepPlugin() = default;

    void on_dllmain() override {
    }

    void on_initialize() override {

    }

    //*******************************************************************************************
    // This is the controller input routine. Everything happens here.
    //*******************************************************************************************
    void on_xinput_get_state(uint32_t* retval, uint32_t user_index, XINPUT_STATE* state) {

        if (state != NULL) {

            /* Title screen fixes */

            const auto pawn = API::get()->get_local_pawn(0);

            if(pawn) {

                if (pawn->get_full_name().starts_with(L"BP_Habitat")) {

                    IsPawn = true;
                }
                else
                {
                    auto& vr = API::get()->param()->vr; /* Prevent camera from moving with arm when menu active */
                    vr->set_aim_method(0);
                    vr->set_decoupled_pitch_enabled(true);
                    vr->set_mod_value("VR_CameraForwardOffset", "0.000000");
                    vr->set_mod_value("VR_CameraRightOffset", "0.000000");
                    vr->set_mod_value("VR_CameraUpOffset", "0.000000");
                    vr->set_mod_value("VR_EnableGUI", "true");
                    IsPawn = false;
                }
            }
            else
            {
                auto& vr = API::get()->param()->vr;  /* Prevent camera from moving with arm when menu active */
                vr->set_aim_method(0);
                vr->set_decoupled_pitch_enabled(true);
                vr->set_mod_value("VR_CameraForwardOffset", "0.000000");
                vr->set_mod_value("VR_CameraRightOffset", "0.000000");
                vr->set_mod_value("VR_CameraUpOffset", "0.000000");
                vr->set_mod_value("VR_EnableGUI", "true");
                IsPawn = false;
            }

            if (IsPawn == true) /* Validate we are in-game */
            {
                if (timer.count<std::chrono::milliseconds>() >= 1000) /* Wait for black bars to disappear before reenabling GUI */
                {
                    auto& vr = API::get()->param()->vr;
                    vr->set_mod_value("VR_EnableGUI", "true");

                    timer.stop();
                    timer.reset();
                }

                /* Configure settings */

                HabitatSavedSettings::get_instance(); /* Disable head bobbing */

                /* Camera fixes */                
    
                const auto HC = HabitatCharacter::get_instance();

                if (HC) {

                    IsExamining = HC->get_bIsRunningCinematic(); /* Check if examining object */
                }

                IsBalancingPrev = IsBalancingCurrent;
                IsAnimationControllingCameraPrev = IsAnimationControllingCameraCurrent;

                const auto ABP = ABP_HabitatCharacterDefault_C::get_instance();

                if (ABP) {

                    IsInteracting = ABP->get_IsInteracting();
                    IsAnimationControllingCameraCurrent = ABP->get_IsAnimationControllingCamera();
                    IsBalancingCurrent = ABP->get_IsBalancing();
                }

                if (IsBalancingPrev != IsBalancingCurrent)
                {
                    if (IsBalancingCurrent == true) /* Adjust height when player is balancing */
                    {
                        reset_height_balancing();
                    }
                    else if (IsBalancingCurrent == false)
                    {
                        reset_height();
                    }
                }

                if (IsAnimationControllingCameraPrev != IsAnimationControllingCameraCurrent)
                {
                    if (IsAnimationControllingCameraCurrent == true) /* Adjust height when animations take over camera position */
                    {
                        reset_height();
                    }
                }

                if ((IsExamining == true) || (IsInteracting == true) || (IsAnimationControllingCameraCurrent == true))  /* Prevent camera from moving with arm and fix UI issues when examining, interacting, and triggering animations */
                {
                    auto& vr = API::get()->param()->vr;
                    vr->set_aim_method(0);
                    vr->set_mod_value("VR_RoomscaleMovement", "false");
                    API::UObjectHook::set_disabled(true);
                }
                else
                {
                    auto& vr = API::get()->param()->vr;
                    vr->set_aim_method(2);

                    if (IsNoPushback == false) {
                        vr->set_mod_value("VR_RoomscaleMovement", "true");
                    }
                    else
                    {
                        vr->set_mod_value("VR_RoomscaleMovement", "false");
                    }

                    API::UObjectHook::set_disabled(false);
                }

                /* Cutscene fixes */

                IsMouseCursorPrev = IsMouseCursorCurrent;

                const auto PC = PlayerController::get_instance();

                if (PC) {

                    IsMouseCursorCurrent = PC->get_bShowMouseCursor(); /* Check if menu is open */
                }

                if (IsMouseCursorPrev != IsMouseCursorCurrent)
                {
                    if (IsMouseCursorCurrent == true) /* Force GUI back on if it was disabled from cutscene fix */
                    {
                        auto& vr = API::get()->param()->vr;
                        vr->set_mod_value("VR_EnableGUI", "true");
                        vr->set_decoupled_pitch_enabled(false);
                    }
                    else if (IsMouseCursorCurrent == false)
                    {
                        auto& vr = API::get()->param()->vr;
                        vr->set_decoupled_pitch_enabled(true);

                        if (IsCutsceneCurrent == true) {
                            vr->set_mod_value("VR_EnableGUI", "false"); /* Hide GUI if resuming cutscene */
                        }
                    }
                }

                IsCutscenePrev = IsCutsceneCurrent;

                const auto HCB = HabitatCharacterBase::get_instance();

                if (HCB) {

                    IsCutsceneCurrent = HCB->get_bIsInCutscene(); /* Check if cutscene is playing */
                }

                if (IsCutscenePrev != IsCutsceneCurrent)
                {
                    if (IsCutsceneCurrent == true)  /* Disable subtitles and black bars during dialog */
                    {
                        auto& vr = API::get()->param()->vr;
                        vr->set_mod_value("VR_EnableGUI", "false");
                        
                    }
                    else
                    {
                        timer.start(); /* GUI reenable timer */
                    }
                }

                /* Add a no pushback mode for VR */

                if (state->Gamepad.bLeftTrigger <= 100)
                {
                    IsNoPushback = false;
                }
                else if (state->Gamepad.bLeftTrigger >= 200)
                {
                    IsNoPushback = true;
                }
            }

            /* Manual height calibration */

            if (state->Gamepad.bLeftTrigger >= 200 && (state->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)) {
                reset_height();
            }
        }
    }
};

void reset_height_balancing() {

    auto& api = API::get();
    auto vr = api->param()->vr;
    UEVR_Vector3f origin{};
    vr->get_standing_origin(&origin);

    UEVR_Vector3f hmd_pos{};
    UEVR_Quaternionf hmd_rot{};
    vr->get_pose(vr->get_hmd_index(), &hmd_pos, &hmd_rot);

    origin.y = (hmd_pos.y) + (float)1.0; /* Recalibrate height for balancing animation */

    vr->set_standing_origin(&origin);
}

void reset_height() {

    auto& api = API::get();
    auto vr = api->param()->vr;
    UEVR_Vector3f origin{};
    vr->get_standing_origin(&origin);

    UEVR_Vector3f hmd_pos{};
    UEVR_Quaternionf hmd_rot{};
    vr->get_pose(vr->get_hmd_index(), &hmd_pos, &hmd_rot);

    origin.y = hmd_pos.y;

    vr->set_standing_origin(&origin);
}

// Actually creates the plugin. Very important that this global is created.
// The fact that it's using std::unique_ptr is not important, as long as the constructor is called in some way.
std::unique_ptr<StillWakesTheDeepPlugin> g_plugin{ new StillWakesTheDeepPlugin() };
