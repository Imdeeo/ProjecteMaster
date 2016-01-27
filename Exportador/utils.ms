fn get_LocalFileName =
(
	return "Game\\Data\\"
)

fn get_FileName =
(
	return "F:\\" + (get_LocalFileName())
)

fn get_RelativePath pathString = 
(
		local startFrom = (findString pathString "Data")
		return substring pathString startFrom (pathString.count-startFrom+1)
)

fn checkUserProp usrProp =
(
	local usrDefined = #()
	for i = 1 to $*.count do
	(
		if (getUserProp $*[i] usrProp) == true then append usrDefined $*[i]
	)
	return usrDefined
)

fn dirExist dir =
(
	if (doesFileExist dir) == false then
		makeDir dir all:true
)

fn get_Cores Objs =
(
	local cores = #()
	local areInstances
	append cores Objs[1]
	setUserProp Objs[1] "core_name" Objs[1].name
	for i = 2 to Objs.count do
	(
		areInstances = false
		for j = 1 to cores.count do
		(
			if ((AreNodesInstances Objs[i] cores[j]) == true) then
			(
				areInstances = true
				setUserProp Objs[i] "core_name" cores[j].name
			)
		)
		if areInstances == false then
		(
			append cores Objs[i]
			setUserProp Objs[i] "core_name" Objs[i].name
		)
	)
	return cores
)
