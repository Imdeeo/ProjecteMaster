dofile("Data\\Lua\\Utils\\state_machine.lua")
dofile("Data\\Lua\\Utils\\EnemyUtils.lua")

class "CVisionTestEnemySM"
  function CVisionTestEnemySM:__init(_owner)
    utils_log("VisionTestEnemySM:__init()")
    self.owner = _owner
    self.m_StateMachine = StateMachine.create()
    self.m_Vision = CEnemyVision(_owner)
    self.m_LatestBlockingObjectName = nil

    local scope_state = State.create(
      function (args, _ElapsedTime)
        self:scope_update(args, _ElapsedTime)
      end
    )
    scope_state:add_condition(function ()
      return self.m_Vision:PlayerVisible()
    end, "watch")
    scope_state:set_do_first_function(scopeFirst)

    local watch_state = State.create(
      function (args, _ElapsedTime)
        self:watch_update(args)
      end
    )
    watch_state:add_condition(
      function ()
        return not self.m_Vision:PlayerVisible()
      end,
      "scope"
    )
    watch_state:set_do_first_function(watchFirst)
    watch_state:set_do_end_function(
      function ()
        self.m_LatestBlockingObjectName = nil
      end
    )

    self.m_StateMachine:add_state("scope", scope_state)
    self.m_StateMachine:add_state("watch", watch_state)
    self.m_StateMachine:start()
  end

  function CVisionTestEnemySM:scope_update(args, _ElapsedTime)
    -- rotate constantly
    local owner = args["owner"]
    local d_rot = Quatf()
    d_rot:quat_from_yaw_pitch_roll(1, 0.0, 0.0)
    local rot = owner:get_rotation()
    local target_quat = d_rot * rot
    local actual_rot = rot:slerp(target_quat, _ElapsedTime)
    owner:set_rotation(actual_rot)

    if self.m_Vision.m_BlockingObjectName ~= self.m_LatestBlockingObjectName then
      utils_log("Object blocking the view: " .. self.m_Vision.m_BlockingObjectName)
      self.m_LatestBlockingObjectName = self.m_Vision.m_BlockingObjectName
    end
  end

  function CVisionTestEnemySM:watch_update(args, _ElapsedTime)
    -- turn to face player
    local owner = args["owner"]
    local l_physXManager = CUABEngine.get_instance():get_physX_manager()
    local playerPos = l_physXManager:get_character_controler_pos("player")
    local ownerPos = owner:get_position()
    local ownerRot = owner:get_rotation()
    local direction = playerPos - ownerPos
    direction.y = 0.0
    local forward = owner:get_rotation():get_forward_vector()
    forward.y = 0.0
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
    if cross.y < 0.0 then
      angle_to_turn = -angle_to_turn
    end
    local quat_to_turn = Quatf()
    quat_to_turn:quat_from_yaw_pitch_roll(angle_to_turn, 0.0, 0.0)
    local target_quat = ownerRot * quat_to_turn
    owner:set_rotation(target_quat)
  end

  function scopeFirst()
    utils_log("VisionTestEnemy: looking around")
  end

  function watchFirst()
    utils_log("VisionTestEnemy: watching player")
  end
