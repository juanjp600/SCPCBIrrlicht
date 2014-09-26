#ifndef START_ROOM_INCLUDED
#define START_ROOM_INCLUDED

class start : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		start() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static start* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
