#ifndef ROOM2TESLA_ROOM_INCLUDED
#define ROOM2TESLA_ROOM_INCLUDED

class room2tesla : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room2tesla() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room2tesla* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
