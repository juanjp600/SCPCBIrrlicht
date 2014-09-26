#ifndef ROOM035_ROOM_INCLUDED
#define ROOM035_ROOM_INCLUDED

class room035 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room035() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room035* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
