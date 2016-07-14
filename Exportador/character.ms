fn createCharacters dir =
(	
	file=createfile (dir+"characters.xml")
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<characters>\n" to:file
	format "\t<player name=\"player\" renderable_object=\"Jaheem\" layer=\"solid\" camera_controller=\"MainCamera\" lua_command=\"InitPlayerMove()\"/>\n" to:file
	format "</characters>\n" to:file
	close file
)