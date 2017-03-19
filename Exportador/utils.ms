fn existFile fname = (getfiles fname).count != 0

fn getRelativePath &pathString = 
(
	local startFrom = (findString pathString "Data")
	pathString = substring pathString startFrom (pathString.count-startFrom+1)
)

fn checkUserProp usrProp &usrDefined =
(
	for Obj in $* do
	(
		if (getUserProp Obj usrProp) == true then append usrDefined Obj
	)
)

fn checkUserPropValue Objs usrProp usrPropValue &usrDefined =
(
	for Obj in Objs do
	(
		if (getUserProp Obj usrProp) == usrPropValue then append usrDefined Obj
	)
)

fn dirExist dir =
(
	if (doesFileExist dir) == false then
		makeDir dir all:true
)

fn getCores Objs &cores=
(
	local areInstances
	append cores Objs[1]
	
	for l_Obj in Objs do
	(
		areInstances = false
		for l_Core in cores do
		(
			if ((AreNodesInstances l_Obj l_Core) == true) then
			(
				areInstances = true
				setUserProp l_Obj "core_name" l_Core.name
			)
		)
		if areInstances == false then
		(
			append cores l_Obj
			setUserProp l_Obj "core_name" l_Obj.name
		)
	)
)

struct PerformanceTester
(
    iStartMem = 0,
    iStartTime = 0,
    iStopMem = 0,
    iStopTime = 0,
    
    iCycle = 0,
    
    iResTime = 0,
    iResMem = 0,
    
    function start =
    (
        gc()
        iCycle += 1
        iStartMem = (heapSize - heapFree)
        iStartTime = timeStamp()    
    ),
    
    function stop =
    (
        iStopTime = timeStamp()
        iStopMem = (heapSize - heapFree)

        iResTime += ((iStopTime - iStartTime) / 1000.0)
        iResMem += ((iStopMem - iStartMem) / 1024.0)
    ),
    
    function report =
    (
		
		format "\n------------CLOCK----------------\n"
        format "Total: %sec & %Kb\n" iResTime iResMem
		if iCycle > 1 then
		(
			format "Avg: %sec & %Kb (% its)\n" (iResTime / iCycle) (iResMem / iCycle) iCycle
		)
        format "---------------------------------\n\n"
		
        iCycle = 0
    
        iResTime = 0
        iResMem = 0
    )
)
