#ifndef ROOM2PIT_ROOM_INCLUDED
#define ROOM2PIT_ROOM_INCLUDED

class room2pit : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room2pit() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room2pit* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
