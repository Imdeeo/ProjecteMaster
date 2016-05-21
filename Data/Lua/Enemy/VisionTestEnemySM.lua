dofile("Data\\Lua\\Utils\\state_machine.lua")

class "VisionTestEnemySM"
  function VisionTestEnemySM:__init(owner)
    utils_log("VisionTestEnemySM:__init()")
    self.owner = owner
    self.m_StateMachine = StateMachine.create()
    local scope_state = State.create(function (args, _ElapsedTime)
      self:scope_update(args, _ElapsedTime)
    end)
    scope_state:add_condition(function ()
      return self:player_in_sight()
    end, "watch")
    scope_state:set_do_first_function(scopeFirst)
    --scope_state:set_do_end_function()

    local watch_state = State.create(function (args, _ElapsedTime)
      self:watch_update(args)
    end)
    watch_state:add_condition(function ()
      return not self:player_in_sight()
    end, "scope")
    watch_state:set_do_first_function(watchFirst)
    --watch_state:set_do_end_function()

    self.m_StateMachine:add_state("scope", scope_state)
    self.m_StateMachine:add_state("watch", watch_state)
    self.m_MaxDistance = 10.0
    self.m_MaxAngle = math.pi * 0.25
    self.m_StateMachine:start()
  end

  function VisionTestEnemySM:scope_update(args, _ElapsedTime)
    local owner = args["owner"]
    local d_rot = Quatf()
    d_rot:quat_from_yaw_pitch_roll(1, 0.0, 0.0)
    local rot = owner:get_rotation()
    local target_quat = d_rot * rot
    local actual_rot = rot:slerp(target_quat, _ElapsedTime)
    owner:set_rotation(actual_rot)
  end

  function VisionTestEnemySM:watch_update(args, _ElapsedTime)
    local owner = args["owner"]
    local l_physXManager = CUABEngine.get_instance():get_physX_manager()
    local playerPos = l_physXManager:get_character_controler_pos("player")
    local ownerPos = owner:get_position()
    local ownerRot = owner:get_rotation()
    local direction = playerPos - ownerPos
    direction.y = 0.0
    local forward = owner:get_rotation():get_forward_vector()
    forward.y = 0.0
    --[[if direction:length() ~= 0.0 then
    end
    if forward:length() ~= 0.0 then
    end]]
    if direction:length() == 0.0 or forward:length() == 0.0 then
      utils_log("Zero length vector!")
      return
    end
    direction:normalize(1.0)
    forward:normalize(1.0)
    local dot = direction * forward
    if 1.0 - dot < 0.01 then
      return
    end
    local angle_to_turn = math.acos(dot)
    local cross = direction ^ forward
    if angle_to_turn < 0.01 then
      return
    end
    if cross.y < 0.0 then
      angle_to_turn = -angle_to_turn
    end
    local quat_to_turn = Quatf()
    quat_to_turn:quat_from_yaw_pitch_roll(angle_to_turn, 0.0, 0.0)
    local target_quat = ownerRot * quat_to_turn
    owner:set_rotation(target_quat)
  end

  function VisionTestEnemySM:player_in_sight()
    local l_physXManager = CUABEngine.get_instance():get_physX_manager()
    local playerPos = l_physXManager:get_character_controler_pos("player")
    local ownerPos = self.owner:get_position()
    local dist = playerPos:distance(ownerPos)
    if dist > self.m_MaxDistance then
      return false
    end

    local l_PlayerDirection = playerPos - ownerPos
    l_PlayerDirection:normalize(1.0)
    local l_Forward = self.owner:get_rotation():get_forward_vector()
    local dot = l_Forward * l_PlayerDirection
    if dot < math.cos(self.m_MaxAngle) then
      return false
    end
    return true
  end

  function scopeFirst()
    utils_log("VisionTestEnemy: scoping")
  end

  function watchFirst()
    utils_log("VisionTestEnemy: watching")
  end
