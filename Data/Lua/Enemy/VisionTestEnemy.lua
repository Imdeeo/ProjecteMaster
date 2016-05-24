dofile("Data\\Lua\\Enemy\\visionTestEnemySM.lua")

g_VisionTestEnemies = {}

class "CVisionTestEnemy"
  function CVisionTestEnemy:__init()

  end

  function CVisionTestEnemy:InitEnemy(name)
    local UABEngine = CUABEngine.get_instance()
    local l_Box=UABEngine:get_layer_manager():get_resource("solid"):get_resource(name)
    local l_Component=l_Box:get_component_manager():get_resource("VisionTestEnemyComponent")

    utils_log("VisionTestEnemy:InitEnemy")
    g_VisionTestEnemies[l_Box.name] = self
    if l_Component==nil then
      local l_Component=create_scripted_component(
        "VisionTestEnemyComponent", l_Box,
        "FnOnCreateEnemy",
        "FnOnDestroyEnemy",
        "FnOnUpdateEnemy",
        "FnOnRenderEnemy", "")
      l_Box:get_component_manager():add_resource("VisionTestEnemyComponent",
        l_Component)
    end
    self.m_StateMachine = CVisionTestEnemySM(l_Box)
  end

  function CVisionTestEnemy:UpdateEnemy(_owner, _ElapsedTime)
    local args = {}
    args["owner"] = _owner
    self.m_StateMachine.m_StateMachine:update(args, _ElapsedTime)
  end

function FnOnCreateEnemy(_owner)

end

function FnOnDestroyEnemy()

end

function FnOnUpdateEnemy(_owner, _ElapsedTime)
  g_VisionTestEnemies[_owner.name]:UpdateEnemy(_owner, _ElapsedTime)
end

function FnOnRenderEnemy(_owner, _rm)

end
