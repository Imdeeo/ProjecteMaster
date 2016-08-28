fileIn "math.ms"
fileIn "utils.ms"

fn AddAnimationEnd _file =
(
	local apos = at time (normTime 1) $.position
	apos.z -= 1.634
	CustomToRH &apos
	format "\t<aend name=\"%\" pos=\"% % %\"/>\n" maxFileName apos.x apos.y apos.z to:_file
)

fn ExportAnimationEndPosition _Path =
(
	if $.name == "CATRigHub001" then
	(
		dirExist _Path
		local file
		if existFile (_Path+"animation_ends.xml") then
		(
			file=openFile (_Path+"animation_ends.xml") mode:"r+"
			seek file #eof
			seek file (filepos file - 10)
			AddAnimationEnd file
			format "</aends>\n" to:file
			close file
		) else (
			file=createFile (_Path+"animation_ends.xml")
			format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
			format "<aends>\n" to:file
			AddAnimationEnd file
			format "</aends>\n" to:file
			close file
		)
	) else (
		messageBox "You must select the CAT Rig Hub!" title:"ERROR"
	)
)