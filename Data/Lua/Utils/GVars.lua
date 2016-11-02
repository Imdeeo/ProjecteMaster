--// 
--// Total character height is g_Height+(2*g_Radius), so here it will be 1.8
--// 	 ____
--//	/    \  -> g_Radius		]
--//	|	 |	}				]
--//	|	 |	}				]  TOTAL
--//	|	 |	}  g_Height		] 
--//	|	 |	}				]  HEIGHT
--//	|	 |	}				]
--//	\____/  -> g_Radius		]
--// 

CONTACT_OFFSET = 0.1

--// PLAYER
g_Height = 1.2
g_Radius = 0.3
g_CrouchingHeight = 0.4
g_StandingOffset = (g_Height*0.5)+g_Radius+CONTACT_OFFSET
g_CrouchingOffset = (g_CrouchingHeight*0.5)+g_Radius+CONTACT_OFFSET
g_TotalHeight = g_Height+(g_Radius*2)

--// ENEMIES
g_EnemyHeight = 1.2
g_EnemyRadius = 0.3
g_EnemyCrouchingHeight = 0.4
g_EnemyStandingOffset = (g_Height*0.5)+g_Radius+CONTACT_OFFSET
g_EnemyCrouchingOffset = (g_CrouchingHeight*0.5)+g_Radius+CONTACT_OFFSET

--// Boss
g_EnemyBossHeight = 0.2
g_EnemyBossRadius = 0.3
g_EnemyBossCrouchingHeight = 0.4
g_EnemyBossStandingOffset = (g_Height*0.5)+g_Radius+CONTACT_OFFSET
g_EnemyBossCrouchingOffset = (g_CrouchingHeight*0.5)+g_Radius+CONTACT_OFFSET

g_Gravity = -9.81
