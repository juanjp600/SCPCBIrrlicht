#ifndef ROOM106_ROOM_INCLUDED
#define ROOM106_ROOM_INCLUDED

class room106 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room106() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room106* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
