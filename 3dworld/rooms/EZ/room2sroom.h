#ifndef ROOM2SROOM_ROOM_INCLUDED
#define ROOM2SROOM_ROOM_INCLUDED

class room2sroom : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room2sroom() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room2sroom* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
