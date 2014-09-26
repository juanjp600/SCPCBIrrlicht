#ifndef ROOM3SERVERS_ROOM_INCLUDED
#define ROOM3SERVERS_ROOM_INCLUDED

class room3servers : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room3servers() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room3servers* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
