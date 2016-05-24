function FogAttackFirst(args)
	utils_log("AttackFirst")
end

function FogAttackUpdate(args, _ElapsedTime)
	color = CColor(1,1,1,1)
	coord = Vect2f(500,300)
	gui_manager:do_text("fontTest", "MUERE", coord, CGUIManager.mid_center, color)
end

function FogAttackEnd(args)
	utils_log("AttackEnd")
end

function FogAttackToChaseCondition()
	return false
end

function FogAttackToOffCondition()
	return m_CharacterManager.m_Enemics[1].m_off == true
end
