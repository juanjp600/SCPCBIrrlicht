#ifndef LOCKROOM2_ROOM_INCLUDED
#define LOCKROOM2_ROOM_INCLUDED

class lockroom2 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		lockroom2() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static lockroom2* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
