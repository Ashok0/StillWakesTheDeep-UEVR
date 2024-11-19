local api = uevr.api
local params = uevr.params
local callbacks = params.sdk.callbacks

local Mactive = false
local Lactive = false
local Playing = false
local is_interacting = false
local in_cutscene = false
local in_running_cut = false
local is_anim_cam = false
local Fpmesh = nil
local Handsmesh = nil
local is_In_Intro = false
local is_traversing = nil
local is_on_ladder = nil
local anim_inst = nil
local is_glas_cut = false
local is_radio = false
local is_pickup = false
local is_throw = false

local function GetMesh()
	--FPMesh
	local skeletal_mesh_c = api:find_uobject("Class /Script/Engine.SkeletalMeshComponent")
    if skeletal_mesh_c ~= nil then
        local skeletal_meshes = skeletal_mesh_c:get_objects_matching(false)

        
        for i, mesh in ipairs(skeletal_meshes) do
            if mesh:get_fname():to_string() == "CharacterMesh0" and string.find(mesh:get_full_name(), "PersistentLevel.BP_HabitatCharacterDefault_C") then
                Fpmesh = mesh
                --print(tostring(Fpmesh:get_full_name()))
                
                break
            end
        end
    end

    --Hands
	local hands_mesh_c = api:find_uobject("Class /Script/Engine.SkeletalMeshComponent")
    if hands_mesh_c ~= nil then
        local hand_meshes = hands_mesh_c:get_objects_matching(false)
        
        
        for i, mesh in ipairs(hand_meshes) do
            if mesh:get_fname():to_string() == "SK_Caz_Hands" and string.find(mesh:get_full_name(), "PersistentLevel.BP_HabitatCharacterDefault_C") then
                Handsmesh = mesh
                --print(tostring(Handsmesh:get_full_name()))
                
                break
            end
        end
    end
end

local function AnimInstance()
	--Anims
	local anim_c = api:find_uobject("Class /Script/Habitat.HabitatCharacterAnimInstance")
    if anim_c ~= nil then
        local anim = anim_c:get_objects_matching(false)

        
        for i, mesh in ipairs(anim) do
            if string.find(mesh:get_full_name(), "PersistentLevel.BP_HabitatCharacterDefault_C") and string.find(mesh:get_full_name(), ".CharacterMesh0.ABP_HabitatCharacterDefault_C") then
                anim_inst = mesh
                --print(tostring(anim_inst:get_full_name()))
                
                break
            end
        end
    end

end

local function HideArms()
	GetMesh()
	Handsmesh:call("SetRenderInMainPass", false)
    Fpmesh:call("SetRenderInMainPass", false)
end

local function ShowArms()
	GetMesh()
	if is_glas_cut == false then
		Fpmesh:call("SetRenderInMainPass", true)
		Handsmesh:call("SetRenderInMainPass", true)
	end
end

uevr.sdk.callbacks.on_pre_engine_tick(function(engine, delta)

    local pawn = api:get_local_pawn(0)
    if pawn ~= nil then
		--anim_inst = pawn.AnimInstance
        in_cutscene = pawn.bIsInCutscene
        in_running_cut = pawn.bIsRunningCinematic
    
		AnimInstance()
		if anim_inst ~= nil then 
			is_anim_cam = anim_inst.IsAnimationControllingCamera
			is_interacting = anim_inst.IsInteracting
			is_traversing = anim_inst.IsTraversing
			is_on_ladder = anim_inst.LadderMantleHasGuideRails
			is_radio = anim_inst.PickupRadio
			is_pickup = anim_inst.PickupStart
			is_throw = anim_inst.PickupThrow
		end
		
		if string.find(pawn:get_full_name(), "Glasgow_A_P.Glasgow_A_P.PersistentLevel.BP_HabitatCharacterDefault_Glasgow_C") then
			is_glas_cut = true
		else
			is_glas_cut = false
		end
		
		if is_anim_cam == true or is_interacting == true or in_cutscene == true or in_running_cut == true or is_traversing == true or is_radio == true or is_pickup == true or is_throw == true then
			if Mactive == false then
				Mactive = true
				Playing = false
				print("Arms Visible")
			end	
				ShowArms()
		else
			if Playing == false then
				HideArms()
				Mactive = false
				Playing = true
				print("Arms Removed")
			end
		end

	end
    
end)
    
    
    
uevr.sdk.callbacks.on_script_reset(function()
	Mactive = false
	Lactive = false
	Playing = false
	is_interacting = false
	in_cutscene = false
	in_running_cut = false
	is_anim_cam = false
	Fpmesh = nil
	Handsmesh = nil
	is_In_Intro = false
	is_traversing = nil
	is_on_ladder = nil
	anim_inst = nil
	is_glas_cut = false
	is_radio = false
	is_pickup = false
	is_throw = false
end)
