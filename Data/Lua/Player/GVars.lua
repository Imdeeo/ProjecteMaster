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

g_Height = 1.2
g_Radius = 0.3
g_CrouchingHeight = 0.4
g_StandingOffset = (g_Height*0.5)+g_Radius
g_CrouchingOffset = (g_CrouchingHeight*0.5)+g_Radius
g_PI = 3.1415926535897932384626433832795