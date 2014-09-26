#ifndef LOCKROOM2_ROOM_INCLUDED
#define LOCKROOM2_ROOM_INCLUDED

class lockroom2 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		lockroom2() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static lockroom2* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
