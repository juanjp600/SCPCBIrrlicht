#ifndef ROOM2SERVERS_ROOM_INCLUDED
#define ROOM2SERVERS_ROOM_INCLUDED

class room2servers : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room2servers() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room2servers* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
