#ifndef START_ROOM_INCLUDED
#define START_ROOM_INCLUDED

class RoomStart : public Room {
	private:
		static RMesh* baseRMesh;
		RoomStart() {};
		irr::scene::IAnimatedMeshSceneNode* sceneNodes[2];
		class Door* doors[2];
		unsigned short state = 0;
		class Sound* sounds[2];
		unsigned char sndChannels[2];
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM1; }
		virtual void updateEvent();
		virtual void setIrrNode(unsigned char index,irr::scene::ISceneNode* node);
		virtual void setDoor(unsigned char index,class Door* door);
		static void setBase(RMesh* inRme);
		static RoomStart* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
