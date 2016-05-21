dofile("Data\\Lua\\Enemy\\visionTestEnemySM.lua")

g_Enemies = {}

class "VisionTestEnemy"
  function VisionTestEnemy:__init()

  end

  function VisionTestEnemy:InitEnemy(name)
    local UABEngine = CUABEngine.get_instance()
    local l_Box=UABEngine:get_layer_manager():get_resource("solid"):get_resource(name)
    local l_Component=l_Box:get_component_manager():get_resource("VisionTestEnemyComponent")

    -- TODO: maybe no point of checking for existing component and then creating a new object anyway?
    utils_log("VisionTestEnemy:InitEnemy")
    if l_Box==nil then
      utils_log("l_Box is nil")
    end
    g_Enemies[l_Box.name] = self
    if l_Component==nil then
      local l_Component=create_scripted_component(
        "VisionTestEnemyComponent", l_Box,
        "FnOnCreateEnemy",
        "FnOnDestroyEnemy",
        "FnOnUpdateEnemy",
        "FnOnRenderEnemy", "")
      if l_Component == nil then
        utils_log("l_Component is nil")
      end
      l_Box:get_component_manager():add_resource("VisionTestEnemyComponent",
        l_Component)
    end
    self.m_StateMachine = VisionTestEnemySM(l_Box)
    --self.m_StateMachine:start()
  end

  function VisionTestEnemy:FnOnCreateEnemy(_owner)
    --local UABEngine = CUABEngine.get_instance()
    --local l_physXManager = UABEngine:get_physX_manager()
    --l_physXManager:register_material("enemyMaterial", 0.5, 0.5, 1)

    --l_physXManager:create_character_controller(
      --"viewEnemy", 1.5, 1, 0.5, _owner:get_position(), "enemyMaterial", 1)
  end

  function VisionTestEnemy:FnOnDestroyEnemy()

  end

  function VisionTestEnemy:FnOnUpdateEnemy(_owner, _ElapsedTime)
    local args = {}
    args["owner"] = _owner
    self.m_StateMachine.m_StateMachine:update(args, _ElapsedTime)
  end

  function VisionTestEnemy.FnOnRenderEnemy(_owner, _rm)

  end

  function FnOnCreateEnemy(_owner)
    if _owner == nil then
      utils_log("_owner was nil")
    end
    local enemy = g_Enemies[_owner.name]
    if enemy == nil then
      utils_log("Couldn't find enemy by owner")
    end
    --g_Enemies[_owner].FnOnCreateEnemy(_owner)
  end

  function FnOnDestroyEnemy()

  end

  function FnOnUpdateEnemy(_owner, _ElapsedTime)
    g_Enemies[_owner.name]:FnOnUpdateEnemy(_owner, _ElapsedTime)
  end

  function FnOnRenderEnemy(_owner, _rm)

  end
