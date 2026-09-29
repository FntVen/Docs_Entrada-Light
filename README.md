OS Level Dependencies
----------------------------------------------------------------------------------------------------
Zip must be installed for non windows devices (Its packed in on macos but not always on linux) (Provide a easy install command later or provide lua fallback in worst case scenario)

Mac does not have a sfx respective, so it will only receive a folder (Main.exe | LuaScripts | CompiledJS for HTTPS calls)

Implementation Details
-----------------------------------------------------------------------------------------------------
Maps  be done by spinning a headless browser already set to G.maps with js, and then execute code that takes a Screenshot

For Autocad Support make a helper Autocad plugging that will read the information outputted by this program (.HEHelper) and modify the .cad template  

To-Do
-----------------------------------------------------------------------------------------------------
Make the function to clean static text not give a segmentation fault
Set clean only for screen resizing and screen changes
Set TextBoxes
Finish Menu Navigation
Set Dark/Light Mode
Find a good api for maps, if i can't, allow user to dump a png/jpg/jpeg and use that
