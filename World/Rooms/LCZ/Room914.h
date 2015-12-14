#ifndef R_914_ROOM_INCLUDED
#define R_914_ROOM_INCLUDED

class Room914 : public Room {
	private:
		static RMesh* baseRMesh;
		Room914() {};
		class Sound* sounds[2];
		unsigned char sndChannels[2];
		irr::core::aabbox3df intake;
		irr::core::aabbox3df output;
		irr::scene::IMeshSceneNode* sceneNodes[2];
		short state = 0;
		class Door* doors[3];

		irr::core::vector3df knobAngle1; irr::core::vector3df knobAngle2;
		irr::core::vector3df knobPosition;
		irr::core::vector3df knobSlidingRot;

		irr::core::vector3df keyAngle1; irr::core::vector3df keyAngle2;
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM1; }
		virtual void updateEvent();
		virtual void setIrrNode(unsigned char index,irr::scene::ISceneNode* node);
		virtual void setDoor(unsigned char index,class Door* door);
		static void setBase(RMesh* inRme);
		static Room914* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
