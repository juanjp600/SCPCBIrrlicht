#ifndef LOCKROOM_ROOM_INCLUDED
#define LOCKROOM_ROOM_INCLUDED

class lockroom : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		lockroom() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static lockroom* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
