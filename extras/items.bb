;crappy script to convert itemtemplates into C++ subclasses for the Irrlicht Port

Global EnumFile% = WriteFile("ENUM.txt")
Global InitsFile% = WriteFile("INITS.txt")
Global IncludesFile% = WriteFile("INCLUDES.txt")

it = CreateItemTemplate("Some SCP-420-J", "420", "GFX\items\420.x", "GFX\items\INV420.jpg", "", 2)
;it\sound = 2

CreateItemTemplate("Level 1 Key Card", "key1",  "GFX\items\keycard.x", "GFX\items\INVkey1.jpg", "", 1,"GFX\items\keycard1.jpg")
CreateItemTemplate("Level 2 Key Card", "key2",  "GFX\items\keycard.x", "GFX\items\INVkey2.jpg", "", 1,"GFX\items\keycard2.jpg")
CreateItemTemplate("Level 3 Key Card", "key3",  "GFX\items\keycard.x", "GFX\items\INVkey3.jpg", "", 1,"GFX\items\keycard3.jpg")
CreateItemTemplate("Level 4 Key Card", "key4",  "GFX\items\keycard.x", "GFX\items\INVkey4.jpg", "", 1,"GFX\items\keycard4.jpg")
CreateItemTemplate("Level 5 Key Card", "key5", "GFX\items\keycard.x", "GFX\items\INVkey5.jpg", "", 1,"GFX\items\keycard5.jpg")
CreateItemTemplate("Playing Card", "misc", "GFX\items\keycard.x", "GFX\items\INVcard.jpg", "", 1,"GFX\items\card.jpg")
CreateItemTemplate("Mastercard", "misc", "GFX\items\keycard.x", "GFX\items\INVmastercard.jpg", "", 1,"GFX\items\mastercard.jpg")
CreateItemTemplate("Key Card Omni", "key6", "GFX\items\keycard.x", "GFX\items\INVkeyomni.jpg", "", 1,"GFX\items\keycardomni.jpg")

it = CreateItemTemplate("SCP-860", "scp860", "GFX\items\key.b3d", "GFX\items\INVkey.jpg", "", 3)
;it\sound = 3

it = CreateItemTemplate("Document SCP-079", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc079.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-895", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc895.jpg", 0) : ;it\sound = 0 
it = CreateItemTemplate("Document SCP-860", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc860.jpg", 0) : ;it\sound = 0 	
it = CreateItemTemplate("Document SCP-860-1", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc8601.jpg", 0) : ;it\sound = 0 	
it = CreateItemTemplate("SCP-093 Recovered Materials", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc093rm.jpg", 0) : ;it\sound = 0 	
it = CreateItemTemplate("Document SCP-106", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc106.jpg", 0) : ;it\sound = 0	
it = CreateItemTemplate("Dr. Allok's Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc106_2.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Recall Protocol RP-106-N", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docRP.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-682", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc682.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-173", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc173.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-372", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc372.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-049", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc049.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-096", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc096.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-008", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc008.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-012", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc012.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-714", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc714.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-513", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc513.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-035", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc035.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("SCP-035 Addendum", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc035ad.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-939", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc939.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-970", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc970.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document SCP-1048", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc1048.jpg", 0) : ;it\sound = 0

it = CreateItemTemplate("Dr. L's Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docL1.jpg", 0, "GFX\items\notetexture.jpg") : ;it\sound = 0
it = CreateItemTemplate("Dr L's Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docL2.jpg", 0, "GFX\items\notetexture.jpg") : ;it\sound = 0
it = CreateItemTemplate("Blood-stained Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docL3.jpg", 0, "GFX\items\notetexture.jpg") : ;it\sound = 0
it = CreateItemTemplate("Dr. L's Burnt Note", "paper", "GFX\items\paper.x", "GFX\items\INVbn.jpg", "GFX\items\docL4.jpg", 0, "GFX\items\BurntNoteTexture.jpg") : ;it\sound = 0
it = CreateItemTemplate("Dr L's Burnt Note", "paper", "GFX\items\paper.x", "GFX\items\INVbn.jpg", "GFX\items\docL5.jpg", 0, "GFX\items\BurntNoteTexture.jpg") : ;it\sound = 0
it = CreateItemTemplate("Scorched Note", "paper", "GFX\items\paper.x", "GFX\items\INVbn.jpg", "GFX\items\docL6.jpg", 0, "GFX\items\BurntNoteTexture.jpg") : ;it\sound = 0

it = CreateItemTemplate("Log #1", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\f4.jpg", 0, "GFX\items\f4.jpg") : ;it\sound = 0
it = CreateItemTemplate("Log #2", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\f5.jpg", 0, "GFX\items\f4.jpg") : ;it\sound = 0
it = CreateItemTemplate("Log #3", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\f6.jpg", 0, "GFX\items\f4.jpg") : ;it\sound = 0

it = CreateItemTemplate("Strange Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docStrange.jpg", 0, "GFX\items\notetexture.jpg") : ;it\sound = 0

it = CreateItemTemplate("Nuclear Device Document", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docNDP.jpg", 0) : ;it\sound = 0	
it = CreateItemTemplate("Class D Orientation Leaflet", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docORI.jpg", 0) : ;it\sound = 0	

it = CreateItemTemplate("Note from Daniel", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docdan.jpg", 0, "GFX\items\notetexture.jpg") : ;it\sound = 0			

it = CreateItemTemplate("Burnt Note", "paper", "GFX\items\paper.x", "GFX\items\INVbn.jpg", "GFX\items\bn.it", 0, "GFX\items\BurntNoteTexture.jpg")
;it\img = BurntNote : ;it\sound = 0

it = CreateItemTemplate("Mysterious Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\sn.it", 0, "GFX\items\notetexture.jpg") : ;it\sound = 0	

it = CreateItemTemplate("Mobile Task Forces", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docMTF.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Mobile Task Force Epsilon-11", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docNTF.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Security Clearance Levels", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docSC.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Object Classes", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docOBJC.jpg", 0) : ;it\sound = 0
it = CreateItemTemplate("Document", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docRAND3.jpg", 0) : ;it\sound = 0 
it = CreateItemTemplate("Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docRAND2.jpg", 0, "GFX\items\notetexture.jpg") : ;it\sound = 0 
it = CreateItemTemplate("Notification", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docRAND1.jpg", 0, "GFX\items\notetexture.jpg") :;it\sound = 0 	
it = CreateItemTemplate("Incident Report SCP-106-0204", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docIR106.jpg", 0) : ;it\sound = 0 

it = CreateItemTemplate("Ballistic Vest", "vest", "GFX\items\vest.x", "GFX\items\INVvest.jpg", "", 2,"GFX\npcs\MTFbody.jpg") : ;it\sound = 2
it = CreateItemTemplate("Heavy Ballistic Vest", "finevest", "GFX\items\vest.x", "GFX\items\INVvest.jpg", "", 2,"GFX\npcs\MTFbody.jpg")
;it\sound = 2
it = CreateItemTemplate("Bulky Ballistic Vest", "veryfinevest", "GFX\items\vest.x", "GFX\items\INVvest.jpg", "", 2,"GFX\npcs\MTFbody.jpg")
;it\sound = 2

it = CreateItemTemplate("Hazmat Suit", "hazmatsuit", "GFX\items\hazmat.b3d", "GFX\items\INVhazmat.jpg", "", 2)
;it\sound = 2
it = CreateItemTemplate("Hazmat Suit", "hazmatsuit2", "GFX\items\hazmat.b3d", "GFX\items\INVhazmat.jpg", "", 2)
;it\sound = 2
it = CreateItemTemplate("Heavy Hazmat Suit", "hazmatsuit3", "GFX\items\hazmat.b3d", "GFX\items\INVhazmat.jpg", "", 2)
;it\sound = 2

it = CreateItemTemplate("cup", "cup", "GFX\items\cup.x", "GFX\items\INVcup.jpg", "", 2) : ;it\sound = 2

it = CreateItemTemplate("Empty Cup", "emptycup", "GFX\items\cup.x", "GFX\items\INVcup.jpg", "", 2) : ;it\sound = 2	

it = CreateItemTemplate("SCP-500-01", "scp500", "GFX\items\pill.b3d", "GFX\items\INVpill.jpg", "", 2) : ;it\sound = 2

it = CreateItemTemplate("First Aid Kit", "firstaid", "GFX\items\firstaid.x", "GFX\items\INVfirstaid.jpg", "", 1)
it = CreateItemTemplate("Small First Aid Kit", "finefirstaid", "GFX\items\firstaid.x", "GFX\items\INVfirstaid.jpg", "", 1)
it = CreateItemTemplate("Blue First Aid Kit", "firstaid2", "GFX\items\firstaid.x", "GFX\items\INVfirstaid2.jpg", "", 1, "GFX\items\firstaidkit2.jpg")
it = CreateItemTemplate("Strange Bottle", "veryfinefirstaid", "GFX\items\eyedrops.b3d", "GFX\items\INVbottle.jpg", "", 1, "GFX\items\bottle.jpg")	

it = CreateItemTemplate("Gas Mask", "gasmask", "GFX\items\gasmask.b3d", "GFX\items\INVgasmask.jpg", "", 2) : ;it\sound = 2
it = CreateItemTemplate("Gas Mask", "supergasmask", "GFX\items\gasmask.b3d", "GFX\items\INVgasmask.jpg", "", 2) : ;it\sound = 2
it = CreateItemTemplate("Heavy Gas Mask", "gasmask3", "GFX\items\gasmask.b3d", "GFX\items\INVgasmask.jpg", "", 2) : ;it\sound = 2

CreateItemTemplate("9V Battery", "bat", "GFX\items\battery.x", "GFX\items\INVbattery.jpg", "", 1)

it = CreateItemTemplate("Origami", "misc", "GFX\items\origami.b3d", "GFX\items\INVorigami.jpg", "", 0) : ;it\sound = 0

CreateItemTemplate("Electronical components", "misc", "GFX\items\electronics.x", "GFX\items\INVelectronics.jpg", "", 1)

it = CreateItemTemplate("Metal Panel", "scp148", "GFX\items\metalpanel.x", "GFX\items\INVmetalpanel.jpg", "", 2) : ;it\sound = 2
it = CreateItemTemplate("SCP-148 Ingot", "scp148ingot", "GFX\items\scp148.x", "GFX\items\INVscp148.jpg", "", 2) : ;it\sound = 2

CreateItemTemplate("S-NAV 300 Navigator", "nav", "GFX\items\navigator.x", "GFX\items\INVnavigator.jpg", "GFX\items\navigator.png", 1)
CreateItemTemplate("S-NAV Navigator", "nav", "GFX\items\navigator.x", "GFX\items\INVnavigator.jpg", "GFX\items\navigator.png", 1)
CreateItemTemplate("S-NAV Navigator Ultimate", "nav", "GFX\items\navigator.x", "GFX\items\INVnavigator.jpg", "GFX\items\navigator.png", 1)
CreateItemTemplate("S-NAV 310 Navigator", "nav", "GFX\items\navigator.x", "GFX\items\INVnavigator.jpg", "GFX\items\navigator.png", 1)

CreateItemTemplate("Radio Transceiver", "radio", "GFX\items\radio.x", "GFX\items\INVradio.jpg", "GFX\items\radioHUD.png", 1);0.0010)
CreateItemTemplate("Radio Transceiver", "fineradio", "GFX\items\radio.x", "GFX\items\INVradio.jpg", "GFX\items\radioHUD.png", 1)
CreateItemTemplate("Radio Transceiver", "veryfineradio", "GFX\items\radio.x", "GFX\items\INVradio.jpg", "GFX\items\radioHUD.png", 1)
CreateItemTemplate("Radio Transceiver", "18vradio", "GFX\items\radio.x", "GFX\items\INVradio.jpg", "GFX\items\radioHUD.png", 1)

it = CreateItemTemplate("Cigarette", "cigarette", "GFX\items\420.x", "GFX\items\INV420.jpg", "", 2) : ;it\sound = 2

it = CreateItemTemplate("Joint", "420s", "GFX\items\420.x", "GFX\items\INV420.jpg", "", 2) : ;it\sound = 2

it = CreateItemTemplate("Smelly Joint", "420s", "GFX\items\420.x", "GFX\items\INV420.jpg", "", 2) : ;it\sound = 2

it = CreateItemTemplate("Severed Hand", "hand", "GFX\items\severedhand.b3d", "GFX\items\INVhand.jpg", "", 2) : ;it\sound = 2
it = CreateItemTemplate("Severed Hand", "hand2", "GFX\items\severedhand.b3d", "GFX\items\INVhand.jpg", "", 2, "GFX\items\shand2.png") : ;it\sound = 2

CreateItemTemplate("18V Battery", "18vbat", "GFX\items\battery.x", "GFX\items\INVbattery.jpg", "", 1)

CreateItemTemplate("Strange Battery", "killbat", "GFX\items\battery.x", "GFX\items\INVbattery.jpg", "", 1)
CreateItemTemplate("Eyedrops", "fineeyedrops", "GFX\items\eyedrops.b3d", "GFX\items\INVeyedrops.jpg", "", 1)
CreateItemTemplate("Eyedrops", "supereyedrops", "GFX\items\eyedrops.b3d", "GFX\items\INVeyedrops.jpg", "", 1)
CreateItemTemplate("ReVision Eyedrops", "eyedrops","GFX\items\eyedrops.b3d", "GFX\items\INVeyedrops.jpg", "", 1)
CreateItemTemplate("RedVision Eyedrops", "eyedrops", "GFX\items\eyedrops.b3d", "GFX\items\INVeyedropsred.jpg", "", 1,"GFX\items\eyedropsred.jpg")

it = CreateItemTemplate("SCP-714", "scp714", "GFX\items\scp714.b3d", "GFX\items\INV714.jpg", "", 3)
;it\sound = 3

it = CreateItemTemplate("SCP-1025", "scp1025", "GFX\items\scp1025.b3d", "GFX\items\INV1025.jpg", "", 0)
;it\sound = 0

it = CreateItemTemplate("SCP-513", "scp513", "GFX\items\513.x", "GFX\items\INV513.jpg", "", 2)
;it\sound = 2

CloseFile EnumFile

Function CreateItemTemplate%(name$, tempname$, objpath$, invimgpath$, imgpath$, sound%, texturepath$ = "", Bump$="")
	tempname = Upper(Left(tempname,1))+Right(tempname,Len(tempname)-1)
	CreateDir "item"+tempname
	
	iheaderFile% = ReadFile("itemTEMPNAME.h")
	isourceFile% = ReadFile("itemTEMPNAME.cpp")
	ChangeDir "item"+tempname
	
	oheaderFile% = WriteFile("item"+tempname+".h")
	osourceFile% = WriteFile("item"+tempname+".cpp")
	
	While Not Eof(iheaderFile)
		WriteLine oheaderFile,Replace(Replace(ReadLine(iheaderFile),"TEMPNAME",tempname),"TMPNAME",Upper(tempname))
	Wend
	While Not Eof(isourceFile)
		WriteLine osourceFile,Replace(Replace(ReadLine(isourceFile),"TEMPNAME",tempname),"ITEMNAME",name)
	Wend
	
	CloseFile oheaderFile
	CloseFile osourceFile
	
	ChangeDir ".."
	CloseFile iheaderFile
	CloseFile isourceFile
	
	WriteLine EnumFile,"ITEM_"+Upper(tempname)+","
End Function

it = ittScaleMeshTex("Some SCP-420-J", "420", "GFX\items\420.x", "GFX\items\INV420.jpg", "", 0.0005)
; = 2

ittScaleMeshTex("Level 1 Key Card", "key1",  "GFX\items\keycard.x", "GFX\items\INVkey1.jpg", "", 0.0004,"GFX\items\keycard1.jpg")
ittScaleMeshTex("Level 2 Key Card", "key2",  "GFX\items\keycard.x", "GFX\items\INVkey2.jpg", "", 0.0004,"GFX\items\keycard2.jpg")
ittScaleMeshTex("Level 3 Key Card", "key3",  "GFX\items\keycard.x", "GFX\items\INVkey3.jpg", "", 0.0004,"GFX\items\keycard3.jpg")
ittScaleMeshTex("Level 4 Key Card", "key4",  "GFX\items\keycard.x", "GFX\items\INVkey4.jpg", "", 0.0004,"GFX\items\keycard4.jpg")
ittScaleMeshTex("Level 5 Key Card", "key5", "GFX\items\keycard.x", "GFX\items\INVkey5.jpg", "", 0.0004,"GFX\items\keycard5.jpg")
ittScaleMeshTex("Playing Card", "misc", "GFX\items\keycard.x", "GFX\items\INVcard.jpg", "", 0.0004,"GFX\items\card.jpg")
ittScaleMeshTex("Mastercard", "misc", "GFX\items\keycard.x", "GFX\items\INVmastercard.jpg", "", 0.0004,"GFX\items\mastercard.jpg")
ittScaleMeshTex("Key Card Omni", "key6", "GFX\items\keycard.x", "GFX\items\INVkeyomni.jpg", "", 0.0004,"GFX\items\keycardomni.jpg")

it = ittScaleMeshTex("SCP-860", "scp860", "GFX\items\key.b3d", "GFX\items\INVkey.jpg", "", 0.001)
; = 3

it = ittScaleMeshTex("Document SCP-079", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc079.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document SCP-895", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc895.jpg", 0.003) : ; = 0 
it = ittScaleMeshTex("Document SCP-860", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc860.jpg", 0.003) : ; = 0 	
it = ittScaleMeshTex("Document SCP-860-1", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc8601.jpg", 0.003) : ; = 0 	
it = ittScaleMeshTex("SCP-093 Recovered Materials", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc093rm.jpg", 0.003) : ; = 0 	
it = ittScaleMeshTex("Document SCP-106", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc106.jpg", 0.003) : ; = 0	
it = ittScaleMeshTex("Dr. Allok's Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc106_2.jpg", 0.0025) : ; = 0
it = ittScaleMeshTex("Recall Protocol RP-106-N", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docRP.jpg", 0.0025) : ; = 0
it = ittScaleMeshTex("Document SCP-682", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc682.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document SCP-173", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc173.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document SCP-372", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc372.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document SCP-049", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc049.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document SCP-096", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc096.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document SCP-008", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc008.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document SCP-012", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc012.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document SCP-714", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc714.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document SCP-513", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc513.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document SCP-035", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc035.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("SCP-035 Addendum", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc035ad.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document SCP-939", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc939.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document SCP-970", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc970.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document SCP-1048", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\doc1048.jpg", 0.003) : ; = 0

it = ittScaleMeshTex("Dr. L's Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docL1.jpg", 0.0025, "GFX\items\notetexture.jpg") : ; = 0
it = ittScaleMeshTex("Dr L's Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docL2.jpg", 0.0025, "GFX\items\notetexture.jpg") : ; = 0
it = ittScaleMeshTex("Blood-stained Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docL3.jpg", 0.0025, "GFX\items\notetexture.jpg") : ; = 0
it = ittScaleMeshTex("Dr. L's Burnt Note", "paper", "GFX\items\paper.x", "GFX\items\INVbn.jpg", "GFX\items\docL4.jpg", 0.0025, "GFX\items\BurntNoteTexture.jpg") : ; = 0
it = ittScaleMeshTex("Dr L's Burnt Note", "paper", "GFX\items\paper.x", "GFX\items\INVbn.jpg", "GFX\items\docL5.jpg", 0.0025, "GFX\items\BurntNoteTexture.jpg") : ; = 0
it = ittScaleMeshTex("Scorched Note", "paper", "GFX\items\paper.x", "GFX\items\INVbn.jpg", "GFX\items\docL6.jpg", 0.0025, "GFX\items\BurntNoteTexture.jpg") : ; = 0

it = ittScaleMeshTex("Log #1", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\f4.jpg", 0.008, "GFX\items\f4.jpg") : ; = 0
it = ittScaleMeshTex("Log #2", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\f5.jpg", 0.008, "GFX\items\f4.jpg") : ; = 0
it = ittScaleMeshTex("Log #3", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\f6.jpg", 0.008, "GFX\items\f4.jpg") : ; = 0

it = ittScaleMeshTex("Strange Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docStrange.jpg", 0.0025, "GFX\items\notetexture.jpg") : ; = 0

it = ittScaleMeshTex("Nuclear Device Document", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docNDP.jpg", 0.003) : ; = 0	
it = ittScaleMeshTex("Class D Orientation Leaflet", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docORI.jpg", 0.003) : ; = 0	

it = ittScaleMeshTex("Note from Daniel", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docdan.jpg", 0.0025, "GFX\items\notetexture.jpg") : ; = 0			

it = ittScaleMeshTex("Burnt Note", "paper", "GFX\items\paper.x", "GFX\items\INVbn.jpg", "GFX\items\bn.it", 0.003, "GFX\items\BurntNoteTexture.jpg")
;it\img = BurntNote : ; = 0

it = ittScaleMeshTex("Mysterious Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\sn.it", 0.003, "GFX\items\notetexture.jpg") : ; = 0	

it = ittScaleMeshTex("Mobile Task Forces", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docMTF.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Mobile Task Force Epsilon-11", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docNTF.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Security Clearance Levels", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docSC.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Object Classes", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docOBJC.jpg", 0.003) : ; = 0
it = ittScaleMeshTex("Document", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docRAND3.jpg", 0.003) : ; = 0 
it = ittScaleMeshTex("Note", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docRAND2.jpg", 0.003, "GFX\items\notetexture.jpg") : ; = 0 
it = ittScaleMeshTex("Notification", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docRAND1.jpg", 0.003, "GFX\items\notetexture.jpg") :; = 0 	
it = ittScaleMeshTex("Incident Report SCP-106-0204", "paper", "GFX\items\paper.x", "GFX\items\INVpaper.jpg", "GFX\items\docIR106.jpg", 0.003) : ; = 0 

it = ittScaleMeshTex("Ballistic Vest", "vest", "GFX\items\vest.x", "GFX\items\INVvest.jpg", "", 0.02,"GFX\npcs\MTFbody.jpg") : ; = 2
it = ittScaleMeshTex("Heavy Ballistic Vest", "finevest", "GFX\items\vest.x", "GFX\items\INVvest.jpg", "", 0.022,"GFX\npcs\MTFbody.jpg")
; = 2
it = ittScaleMeshTex("Bulky Ballistic Vest", "veryfinevest", "GFX\items\vest.x", "GFX\items\INVvest.jpg", "", 0.025,"GFX\npcs\MTFbody.jpg")
; = 2

it = ittScaleMeshTex("Hazmat Suit", "hazmatsuit", "GFX\items\hazmat.b3d", "GFX\items\INVhazmat.jpg", "", 0.013)
; = 2
it = ittScaleMeshTex("Hazmat Suit", "hazmatsuit2", "GFX\items\hazmat.b3d", "GFX\items\INVhazmat.jpg", "", 0.013)
; = 2
it = ittScaleMeshTex("Heavy Hazmat Suit", "hazmatsuit3", "GFX\items\hazmat.b3d", "GFX\items\INVhazmat.jpg", "", 0.013)
; = 2

it = ittScaleMeshTex("cup", "cup", "GFX\items\cup.x", "GFX\items\INVcup.jpg", "", 0.04) : ; = 2

it = ittScaleMeshTex("Empty Cup", "emptycup", "GFX\items\cup.x", "GFX\items\INVcup.jpg", "", 0.04) : ; = 2	

it = ittScaleMeshTex("SCP-500-01", "scp500", "GFX\items\pill.b3d", "GFX\items\INVpill.jpg", "", 0.0010) : ; = 2

it = ittScaleMeshTex("First Aid Kit", "firstaid", "GFX\items\firstaid.x", "GFX\items\INVfirstaid.jpg", "", 0.05)
it = ittScaleMeshTex("Small First Aid Kit", "finefirstaid", "GFX\items\firstaid.x", "GFX\items\INVfirstaid.jpg", "", 0.03)
it = ittScaleMeshTex("Blue First Aid Kit", "firstaid2", "GFX\items\firstaid.x", "GFX\items\INVfirstaid2.jpg", "", 0.03, "GFX\items\firstaidkit2.jpg")
it = ittScaleMeshTex("Strange Bottle", "veryfinefirstaid", "GFX\items\eyedrops.b3d", "GFX\items\INVbottle.jpg", "", 0.002, "GFX\items\bottle.jpg")	

it = ittScaleMeshTex("Gas Mask", "gasmask", "GFX\items\gasmask.b3d", "GFX\items\INVgasmask.jpg", "", 0.02) : ; = 2
it = ittScaleMeshTex("Gas Mask", "supergasmask", "GFX\items\gasmask.b3d", "GFX\items\INVgasmask.jpg", "", 0.021) : ; = 2
it = ittScaleMeshTex("Heavy Gas Mask", "gasmask3", "GFX\items\gasmask.b3d", "GFX\items\INVgasmask.jpg", "", 0.021) : ; = 2

ittScaleMeshTex("9V Battery", "bat", "GFX\items\battery.x", "GFX\items\INVbattery.jpg", "", 0.0002)

it = ittScaleMeshTex("Origami", "misc", "GFX\items\origami.b3d", "GFX\items\INVorigami.jpg", "", 0.003) : ; = 0

ittScaleMeshTex("Electronical components", "misc", "GFX\items\electronics.x", "GFX\items\INVelectronics.jpg", "", 0.0011)

it = ittScaleMeshTex("Metal Panel", "scp148", "GFX\items\metalpanel.x", "GFX\items\INVmetalpanel.jpg", "", 8.0/2048.0) : ; = 2
it = ittScaleMeshTex("SCP-148 Ingot", "scp148ingot", "GFX\items\scp148.x", "GFX\items\INVscp148.jpg", "", 8.0/2048.0) : ; = 2

ittScaleMeshTex("S-NAV 300 Navigator", "nav", "GFX\items\navigator.x", "GFX\items\INVnavigator.jpg", "GFX\items\navigator.png", 0.0008)
ittScaleMeshTex("S-NAV Navigator", "nav", "GFX\items\navigator.x", "GFX\items\INVnavigator.jpg", "GFX\items\navigator.png", 0.0008)
ittScaleMeshTex("S-NAV Navigator Ultimate", "nav", "GFX\items\navigator.x", "GFX\items\INVnavigator.jpg", "GFX\items\navigator.png", 0.0008)
ittScaleMeshTex("S-NAV 310 Navigator", "nav", "GFX\items\navigator.x", "GFX\items\INVnavigator.jpg", "GFX\items\navigator.png", 0.0008)

ittScaleMeshTex("Radio Transceiver", "radio", "GFX\items\radio.x", "GFX\items\INVradio.jpg", "GFX\items\radioHUD.png", 1.0);0.0010)
ittScaleMeshTex("Radio Transceiver", "fineradio", "GFX\items\radio.x", "GFX\items\INVradio.jpg", "GFX\items\radioHUD.png", 0.0010)
ittScaleMeshTex("Radio Transceiver", "veryfineradio", "GFX\items\radio.x", "GFX\items\INVradio.jpg", "GFX\items\radioHUD.png", 0.0010)
ittScaleMeshTex("Radio Transceiver", "18vradio", "GFX\items\radio.x", "GFX\items\INVradio.jpg", "GFX\items\radioHUD.png", 0.0012)

it = ittScaleMeshTex("Cigarette", "cigarette", "GFX\items\420.x", "GFX\items\INV420.jpg", "", 0.0004) : ; = 2

it = ittScaleMeshTex("Joint", "420s", "GFX\items\420.x", "GFX\items\INV420.jpg", "", 0.0004) : ; = 2

it = ittScaleMeshTex("Smelly Joint", "420s", "GFX\items\420.x", "GFX\items\INV420.jpg", "", 0.0004) : ; = 2

it = ittScaleMeshTex("Severed Hand", "hand", "GFX\items\severedhand.b3d", "GFX\items\INVhand.jpg", "", 0.04) : ; = 2
it = ittScaleMeshTex("Severed Hand", "hand2", "GFX\items\severedhand.b3d", "GFX\items\INVhand.jpg", "", 0.04, "GFX\items\shand2.png") : ; = 2

ittScaleMeshTex("18V Battery", "18vbat", "GFX\items\battery.x", "GFX\items\INVbattery.jpg", "", 0.0003)

ittScaleMeshTex("Strange Battery", "killbat", "GFX\items\battery.x", "GFX\items\INVbattery.jpg", "", 0.0003)
ittScaleMeshTex("Eyedrops", "fineeyedrops", "GFX\items\eyedrops.b3d", "GFX\items\INVeyedrops.jpg", "", 0.0012)
ittScaleMeshTex("Eyedrops", "supereyedrops", "GFX\items\eyedrops.b3d", "GFX\items\INVeyedrops.jpg", "", 0.0012)
ittScaleMeshTex("ReVision Eyedrops", "eyedrops","GFX\items\eyedrops.b3d", "GFX\items\INVeyedrops.jpg", "", 0.0012)
ittScaleMeshTex("RedVision Eyedrops", "eyedrops", "GFX\items\eyedrops.b3d", "GFX\items\INVeyedropsred.jpg", "", 0.0012,"GFX\items\eyedropsred.jpg")

it = ittScaleMeshTex("SCP-714", "scp714", "GFX\items\scp714.b3d", "GFX\items\INV714.jpg", "", 0.3)
; = 3

it = ittScaleMeshTex("SCP-1025", "scp1025", "GFX\items\scp1025.b3d", "GFX\items\INV1025.jpg", "", 0.1)
; = 0

it = ittScaleMeshTex("SCP-513", "scp513", "GFX\items\513.x", "GFX\items\INV513.jpg", "", 0.1)
; = 2

CloseFile InitsFile
CloseFile IncludesFile

Function ittScaleMeshTex%(name$, tempname$, objpath$, invimgpath$, imgpath$, scale#, texturepath$ = "", Bump$="")
	tempname = Upper(Left(tempname,1))+Right(tempname,Len(tempname)-1)
	WriteLine InitsFile,"item"+tempname+"::setMeshNode(genItemNode(std::string("+Chr(34)+objpath+Chr(34)+"),std::string("+Chr(34)+texturepath+Chr(34)+"),"+Str(scale/0.002*0.06)+"f*RoomScale));"
	WriteLine IncludesFile,"#include "+Chr(34)+"item"+tempname+"/item"+tempname+".h"+Chr(34)
End Function
;~IDEal Editor Parameters:
;~C#Blitz3D