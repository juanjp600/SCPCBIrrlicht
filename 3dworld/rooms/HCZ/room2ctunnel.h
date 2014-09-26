#ifndef ROOM2CTUNNEL_ROOM_INCLUDED
#define ROOM2CTUNNEL_ROOM_INCLUDED

class room2ctunnel : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room2ctunnel() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room2ctunnel* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
