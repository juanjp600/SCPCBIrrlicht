#ifndef ROOM4_ROOM_INCLUDED
#define ROOM4_ROOM_INCLUDED

class room4 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room4() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM4; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room4* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
