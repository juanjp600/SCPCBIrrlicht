#ifndef COFFIN_ROOM_INCLUDED
#define COFFIN_ROOM_INCLUDED

class coffin : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		coffin() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static coffin* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
