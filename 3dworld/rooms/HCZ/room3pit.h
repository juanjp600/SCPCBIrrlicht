#ifndef ROOM3PIT_ROOM_INCLUDED
#define ROOM3PIT_ROOM_INCLUDED

class room3pit : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room3pit() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room3pit* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
