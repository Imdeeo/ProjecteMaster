fn getLocalFileName &str =
(
	str = "Game\\Data\\"
)

fn getFileName &str =
(
	getLocalFileName &str
	str = "F:\\" + str
)

fn getRelativePath &pathString = 
(
		local startFrom = (findString pathString "Data")
		pathString = substring pathString startFrom (pathString.count-startFrom+1)
)

fn checkUserProp usrProp &usrDefined =
(
	for i = 1 to $*.count do
	(
		if (getUserProp $*[i] usrProp) == true then append usrDefined $*[i]
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
        format "Average processing on % iteration(s) took:\n" iCycle
        format "% seconds\n" (iResTime / iCycle)
        format "% Kbytes\n\n" (iResMem / iCycle)

        iCycle = 0
    
        iResTime = 0
        iResMem = 0
    )
)
