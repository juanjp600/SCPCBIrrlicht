;This thing parses rooms.ini and writes a bunch of files that can be used for the Irrlicht Port

rf% = ReadFile("rooms.ini")
iwf% = WriteFile("includes.txt")
cmwf% = WriteFile("setBase.txt")
sbwf% = WriteFile("createMap.txt")

helperStr1$ = ""

helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "retRoom->node = baseNode->clone(); retRoom->node->setVisible(true);"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "retRoom->node->setPosition(inPosition);"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "retRoom->node->setRotation(irr::core::vector3df(0,inAngle*90.f,0));"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "retRoom->angle = inAngle;"
helperStr1$ = helperStr1 + Chr(13) + Chr(10)
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "//Add the Bullet rigid body"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "retRoom->node->updateAbsolutePosition();"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "irr::core::vector3df irrPos = retRoom->node->getAbsolutePosition();"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "btVector3 btPos(irrPos.X, irrPos.Y, irrPos.Z);"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "btTransform Transform;"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "Transform.setIdentity();"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "Transform.setOrigin(btPos);"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "btQuaternion btRot;"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "irr::core::vector3df irrRot = retRoom->node->getRotation();"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "btRot.setEulerZYX(irrRot.Z*irr::core::DEGTORAD,irrRot.Y*irr::core::DEGTORAD,irrRot.X*irr::core::DEGTORAD);"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "Transform.setRotation(btRot);"
helperStr1$ = helperStr1 + Chr(13) + Chr(10)
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);"
helperStr1$ = helperStr1 + Chr(13) + Chr(10)
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "btVector3 localInertia;"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "room2::baseRMesh->shape->calculateLocalInertia(0.0, localInertia);"
helperStr1$ = helperStr1 + Chr(13) + Chr(10)
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "retRoom->rbody = new btRigidBody(0.0, MotionState, room2::baseRMesh->shape, localInertia);"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);"
helperStr1$ = helperStr1 + Chr(13) + Chr(10)
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "retRoom->rbody->setFriction(1.f);"
helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "retRoom->rbody->setRollingFriction(1.f);"

helperStr1$ = helperStr1 + Chr(13) + Chr(10) + Chr(9) + "return retRoom;"

Type roomMem
	Field zone$
	Field name$
	Field shape$
	Field commonness%
	Field path$
End Type

While Not Eof(rf)
	nstr$ = ReadLine(rf)
	If Instr(nstr,"[")<>0 Then
		nstr = Lower(Replace(Replace(nstr,"[",""),"]",""))
		If (nstr<>"room ambience") Then
			roomType$ = Upper(GetINIString("rooms.ini",nstr,"shape"))
			zstr$ = GetINIString("rooms.ini",nstr,"zone1","0")
			Select zstr
				Case "1" ;lcz
					zstr = "LCZ"
				Case "2" ;hcz
					zstr = "HCZ"
				Case "3" ;ez
					zstr = "EZ"
				Default
					zstr = "Misc"
					;RuntimeError "Error 1: "+zstr
			End Select
			
			rm.RoomMem = New roomMem
			rm\zone = zstr
			rm\shape = roomType
			rm\commonness = Int(GetINIString("rooms.ini",nstr,"commonness","0"))
			rm\path = GetINIString("rooms.ini",nstr,"mesh path")
			
			ddecals$ = Lower(GetINIString("rooms.ini",nstr,"zone1","false"))
			
			If (Int(Left(nstr,1))<>0 Or Left(nstr,1)="0") nstr="r_"+nstr
				
				rm\name = nstr
			
			ohrf% = WriteFile("outputSrc/"+zstr+"/"+nstr+".h")
			
			WriteLine ohrf,"#ifndef "+Upper(nstr)+"_ROOM_INCLUDED"
			WriteLine ohrf,"#define "+Upper(nstr)+"_ROOM_INCLUDED"
			
			WriteLine ohrf,""
			
			WriteLine ohrf,"class "+nstr+" : public room {"
			WriteLine ohrf,Chr(9)+"private:"
			WriteLine ohrf,Chr(9)+Chr(9)+"static irr::scene::IMeshSceneNode* baseNode;"
			WriteLine ohrf,Chr(9)+Chr(9)+"static RMesh* baseRMesh;"
			WriteLine ohrf,Chr(9)+Chr(9)+nstr+"() {};"
			WriteLine ohrf,Chr(9)+"public:"
			WriteLine ohrf,Chr(9)+Chr(9)+"virtual roomTypes getType() { return roomTypes::ROOM"+roomType+"; }"
			WriteLine ohrf,Chr(9)+Chr(9)+"virtual void updateEvent() { return; }"
			WriteLine ohrf,Chr(9)+Chr(9)+"virtual const std::vector<irr::video::SLight>& getPointLights();"
			WriteLine ohrf,Chr(9)+Chr(9)+"static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);"
			WriteLine ohrf,Chr(9)+Chr(9)+"static "+nstr+"* createNew(irr::core::vector3df inPosition,char inAngle);"
			WriteLine ohrf,"};"
			
			WriteLine ohrf,""
			
			WriteLine ohrf,"#endif"
			
			CloseFile ohrf
			
			osrf% = WriteFile("outputSrc/"+zstr+"/"+nstr+".cpp")
			
			WriteLine osrf,"#include "+Chr(34)+"../room.h"+Chr(34)
			WriteLine osrf,"#include "+Chr(34)+nstr+".h"+Chr(34)
			
			WriteLine osrf,""
			
			WriteLine osrf,"irr::scene::IMeshSceneNode* "+nstr+"::baseNode = nullptr;"
			WriteLine osrf,"RMesh* "+nstr+"::baseRMesh = nullptr;"
			
			WriteLine osrf,""
			
			WriteLine osrf,"void "+nstr+"::setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme) {"
			WriteLine osrf,Chr(9)+"if ("+nstr+"::baseNode==nullptr || "+nstr+"::baseRMesh==nullptr) {"
			WriteLine osrf,Chr(9)+Chr(9)+nstr+"::baseNode = inNode; "+nstr+"::baseRMesh = inRme; "+nstr+"::baseNode->setVisible(false);"
			WriteLine osrf,Chr(9)+"}"
			WriteLine osrf,"}"
			
			WriteLine osrf,""
			
			WriteLine osrf,nstr+"* "+nstr+"::createNew(irr::core::vector3df inPosition,char inAngle) {"
			WriteLine osrf,Chr(9)+nstr+"* retRoom = new "+nstr+";"
			WriteLine osrf,Replace(helperStr1,"room2",nstr)
			WriteLine osrf,"}"
			
			WriteLine osrf,""
			
			WriteLine osrf,"const std::vector<irr::video::SLight>& "+nstr+"::getPointLights() {"
			WriteLine osrf,Chr(9)+"return "+nstr+"::baseRMesh->pointlights;"
			WriteLine osrf,"}"
			
		EndIf
	EndIf
Wend

Local zoneRoomCount%[4]
Local zoneRoom1Commonness%[4]
Local zoneRoom2Commonness%[4]
Local zoneRoom2cCommonness%[4]
Local zoneRoom3Commonness%[4]
Local zoneRoom4Commonness%[4]
For i%=0 To 3
	wantZone$ = ""
	Select i
		Case 0
			wantZone = "LCZ"
		Case 1
			wantZone = "HCZ"
		Case 2
			wantZone = "EZ"
		Default
			wantZone = "Misc"
	End Select
	
	WriteLine iwf,"//"+wantZone
	WriteLine cmwf,"//"+wantZone
	For rm.roomMem = Each roomMem
		If rm\zone = wantZone Then
			WriteLine iwf,"#include "+Chr(34)+rm\zone+"/"+rm\name+".h"+Chr(34)
			rm\path = Replace(rm\path,"\","/")
			WriteLine cmwf,"/*"+rm\name+"*/rme = loadRMesh(std::string("+Chr(34)+rm\path+Chr(34)+"),irrFileSystem,irrDriver); "+rm\name+"::setBase(irrSmgr->addMeshSceneNode(rme->mesh),rme);"
			
			If rm\commonness > 0 Then
				zoneRoomCount[i]=zoneRoomCount[i]+1
				Select rm\shape
					Case "1"
						zoneRoom1Commonness[i]=zoneRoom1Commonness[i]+rm\commonness
					Case "2"
						zoneRoom2Commonness[i]=zoneRoom2Commonness[i]+rm\commonness
					Case "2C"
						zoneRoom2cCommonness[i]=zoneRoom2cCommonness[i]+rm\commonness
					Case "3"
						zoneRoom3Commonness[i]=zoneRoom3Commonness[i]+rm\commonness
					Case "4"
						zoneRoom4Commonness[i]=zoneRoom4Commonness[i]+rm\commonness
				End Select
			EndIf
		EndIf
	Next
Next
Local pg% = 0
WriteLine sbwf,"int choice = 0;"
WriteLine sbwf,"room* retRoom = nullptr;"
WriteLine sbwf,"switch (type) {"
WriteLine sbwf,Chr(9)+"case roomTypes::ROOM1:"
WriteLine sbwf,Chr(9)+Chr(9)+"switch (zone) {"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 0: //LCZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom1Commonness[0]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="LCZ" And rm\shape = "1") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 1: //HCZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom1Commonness[1]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="HCZ" And rm\shape = "1") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 2: //EZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom1Commonness[2]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="EZ" And rm\shape = "1") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+"}"
WriteLine sbwf,Chr(9)+"break;"

WriteLine sbwf,Chr(9)+"case roomTypes::ROOM2:"
WriteLine sbwf,Chr(9)+Chr(9)+"switch (zone) {"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 0: //LCZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom2Commonness[0]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="LCZ" And rm\shape = "2") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 1: //HCZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom2Commonness[1]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="HCZ" And rm\shape = "2") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 2: //EZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom2Commonness[2]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="EZ" And rm\shape = "2") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+"}"
WriteLine sbwf,Chr(9)+"break;"

WriteLine sbwf,Chr(9)+"case roomTypes::ROOM2C:"
WriteLine sbwf,Chr(9)+Chr(9)+"switch (zone) {"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 0: //LCZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom2cCommonness[0]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="LCZ" And rm\shape = "2C") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 1: //HCZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom2cCommonness[1]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="HCZ" And rm\shape = "2C") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 2: //EZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom2cCommonness[2]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="EZ" And rm\shape = "2C") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+"}"
WriteLine sbwf,Chr(9)+"break;"

WriteLine sbwf,Chr(9)+"case roomTypes::ROOM3:"
WriteLine sbwf,Chr(9)+Chr(9)+"switch (zone) {"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 0: //LCZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom3Commonness[0]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="LCZ" And rm\shape = "3") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 1: //HCZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom3Commonness[1]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="HCZ" And rm\shape = "3") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 2: //EZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom3Commonness[2]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="EZ" And rm\shape = "3") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+"}"
WriteLine sbwf,Chr(9)+"break;"

WriteLine sbwf,Chr(9)+"case roomTypes::ROOM4:"
WriteLine sbwf,Chr(9)+Chr(9)+"switch (zone) {"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 0: //LCZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom4Commonness[0]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="LCZ" And rm\shape = "4") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 1: //HCZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom4Commonness[1]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="HCZ" And rm\shape = "4") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"case 2: //EZ"
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"choice = rand()%"+zoneRoom4Commonness[2]+";"
pg = 0
For rm.roomMem = Each roomMem
	If (rm\commonness>0 And rm\zone ="EZ" And rm\shape = "4") Then
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"if (choice>="+pg+" && choice<"+(pg+rm\commonness)+") {"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+Chr(9)+"retRoom = "+rm\name+"::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);"
		WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+Chr(9)+"}"
		pg=pg+rm\commonness
	EndIf
Next
WriteLine sbwf,Chr(9)+Chr(9)+Chr(9)+"break;"
WriteLine sbwf,Chr(9)+Chr(9)+"}"
WriteLine sbwf,Chr(9)+"break;"

WriteLine sbwf,"}"

WriteLine sbwf,"roomArray[x][y] = retRoom;"
WriteLine sbwf,"return retRoom;"

CloseFile rf
CloseFile cmwf
CloseFile iwf
CloseFile sbwf

;--------------------------------------- INI-functions -------------------------------------------------------

Function Min#(a#, b#)
	If a < b Then
		Return a
	Else
		Return b
	EndIf
End Function

Function Max#(a#, b#)
	If a > b Then
		Return a
	Else
		Return b
	EndIf
End Function

Function GetINIString$(file$, section$, parameter$, defaultvalue$="")
	Local TemporaryString$ = ""
	Local f% = ReadFile(file)
	
	section = Lower(section)
	
	While Not Eof(f)
		Local strtemp$ = ReadLine(f)
		If Left(strtemp,1) = "[" Then
			strtemp$ = Lower(strtemp)
			If Mid(strtemp, 2, Len(strtemp)-2)=section Then
				Repeat
					TemporaryString = ReadLine(f)
					If Lower(Trim(Left(TemporaryString, Max(Instr(TemporaryString, "=") - 1, 0)))) = Lower(parameter) Then
						CloseFile f
						Return Trim( Right(TemporaryString,Len(TemporaryString)-Instr(TemporaryString,"=")) )
					EndIf
				Until Left(TemporaryString, 1) = "[" Or Eof(f)
				
				CloseFile f
				Return defaultvalue
			EndIf
		EndIf
	Wend
	
	CloseFile f
	
	Return defaultvalue
End Function

Function GetINIInt%(file$, section$, parameter$, defaultvalue% = 0)
	Local txt$ = GetINIString(file$, section$, parameter$, defaultvalue)
	If Lower(txt) = "true" Then
		Return 1
	ElseIf Lower(txt) = "false"
		Return 0
	Else
		Return Int(txt)
	EndIf
End Function

Function GetINIFloat#(file$, section$, parameter$, defaultvalue# = 0.0)
	Return Float(GetINIString(file$, section$, parameter$, defaultvalue))
End Function


Function GetINIString2$(file$, start%, parameter$, defaultvalue$="")
	Local TemporaryString$ = ""
	Local f% = ReadFile(file)
	
	Local n%=0
	While Not Eof(f)
		Local strtemp$ = ReadLine(f)
		n=n+1
		If n=start Then 
			Repeat
				TemporaryString = ReadLine(f)
				If Lower(Trim(Left(TemporaryString, Max(Instr(TemporaryString, "=") - 1, 0)))) = Lower(parameter) Then
					CloseFile f
					Return Trim( Right(TemporaryString,Len(TemporaryString)-Instr(TemporaryString,"=")) )
				EndIf
			Until Left(TemporaryString, 1) = "[" Or Eof(f)
			CloseFile f
			Return defaultvalue
		EndIf
	Wend
	
	CloseFile f	
	
	Return defaultvalue
End Function

Function GetINIInt2%(file$, start%, parameter$, defaultvalue$="")
	Local txt$ = GetINIString2(file$, start%, parameter$, defaultvalue$)
	If Lower(txt) = "true" Then
		Return 1
	ElseIf Lower(txt) = "false"
		Return 0
	Else
		Return Int(txt)
	EndIf
End Function


Function GetINISectionLocation%(file$, section$)
	Local Temp%
	Local f% = ReadFile(file)
	
	section = Lower(section)
	
	Local n%=0
	While Not Eof(f)
		Local strtemp$ = ReadLine(f)
		n=n+1
		If Left(strtemp,1) = "[" Then
			strtemp$ = Lower(strtemp)
			Temp = Instr(strtemp, section)
			If Temp>0 Then
				If Mid(strtemp, Temp-1, 1)="[" Or Mid(strtemp, Temp-1, 1)="|" Then
					CloseFile f
					Return n
				EndIf
			EndIf
		EndIf
	Wend
	
	CloseFile f
End Function



Function PutINIValue%(file$, INI_sSection$, INI_sKey$, INI_sValue$)
	
	; Returns: True (Success) Or False (Failed)
	
	INI_sSection = "[" + Trim$(INI_sSection) + "]"
	Local INI_sUpperSection$ = Upper$(INI_sSection)
	INI_sKey = Trim$(INI_sKey)
	INI_sValue = Trim$(INI_sValue)
	Local INI_sFilename$ = file$
	
	; Retrieve the INI Data (If it exists)
	
	Local INI_sContents$ = INI_FileToString(INI_sFilename)
	
		; (Re)Create the INI file updating/adding the SECTION, KEY And VALUE
	
	Local INI_bWrittenKey% = False
	Local INI_bSectionFound% = False
	Local INI_sCurrentSection$ = ""
	
	Local INI_lFileHandle% = WriteFile(INI_sFilename)
	If INI_lFileHandle = 0 Then Return False ; Create file failed!
	
	Local INI_lOldPos% = 1
	Local INI_lPos% = Instr(INI_sContents, Chr$(0))
	
	While (INI_lPos <> 0)
		
		Local INI_sTemp$ = Mid$(INI_sContents, INI_lOldPos, (INI_lPos - INI_lOldPos))
		
		If (INI_sTemp <> "") Then
			
			If Left$(INI_sTemp, 1) = "[" And Right$(INI_sTemp, 1) = "]" Then
				
					; Process SECTION
				
				If (INI_sCurrentSection = INI_sUpperSection) And (INI_bWrittenKey = False) Then
					INI_bWrittenKey = INI_CreateKey(INI_lFileHandle, INI_sKey, INI_sValue)
				End If
				INI_sCurrentSection = Upper$(INI_CreateSection(INI_lFileHandle, INI_sTemp))
				If (INI_sCurrentSection = INI_sUpperSection) Then INI_bSectionFound = True
				
			Else
				DebugLog INI_sTemp
				If Left(INI_sTemp, 1) = ":" Then
					WriteLine INI_lFileHandle, INI_sTemp
				Else
						; KEY=VALUE				
					Local lEqualsPos% = Instr(INI_sTemp, "=")
					If (lEqualsPos <> 0) Then
						If (INI_sCurrentSection = INI_sUpperSection) And (Upper$(Trim$(Left$(INI_sTemp, (lEqualsPos - 1)))) = Upper$(INI_sKey)) Then
							If (INI_sValue <> "") Then INI_CreateKey INI_lFileHandle, INI_sKey, INI_sValue
							INI_bWrittenKey = True
						Else
							WriteLine INI_lFileHandle, INI_sTemp
						End If
					End If
				EndIf
				
			End If
			
		End If
		
			; Move through the INI file...
		
		INI_lOldPos = INI_lPos + 1
		INI_lPos% = Instr(INI_sContents, Chr$(0), INI_lOldPos)
		
	Wend
	
		; KEY wasn;t found in the INI file - Append a new SECTION If required And create our KEY=VALUE Line
	
	If (INI_bWrittenKey = False) Then
		If (INI_bSectionFound = False) Then INI_CreateSection INI_lFileHandle, INI_sSection
		INI_CreateKey INI_lFileHandle, INI_sKey, INI_sValue
	End If
	
	CloseFile INI_lFileHandle
	
	Return True ; Success
	
End Function

Function INI_FileToString$(INI_sFilename$)
	
	Local INI_sString$ = ""
	Local INI_lFileHandle%= ReadFile(INI_sFilename)
	If INI_lFileHandle <> 0 Then
		While Not(Eof(INI_lFileHandle))
			INI_sString = INI_sString + ReadLine$(INI_lFileHandle) + Chr$(0)
		Wend
		CloseFile INI_lFileHandle
	End If
	Return INI_sString
	
End Function

Function INI_CreateSection$(INI_lFileHandle%, INI_snewSection$)
	
	If FilePos(INI_lFileHandle) <> 0 Then WriteLine INI_lFileHandle, "" ; Blank Line between sections
	WriteLine INI_lFileHandle, INI_snewSection
	Return INI_snewSection
	
End Function

Function INI_CreateKey%(INI_lFileHandle%, INI_sKey$, INI_sValue$)
	
	WriteLine INI_lFileHandle, INI_sKey + " = " + INI_sValue
	Return True
	
End Function

;~IDEal Editor Parameters:
;~C#Blitz3D