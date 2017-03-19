fn createCharacters dir =
(	
	file=createfile (dir+"characters.xml")
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<characters>\n" to:file
	
	format "</characters>\n" to:file
	close file
)