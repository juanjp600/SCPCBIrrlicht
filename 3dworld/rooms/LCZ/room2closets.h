#ifndef ROOM2CLOSETS_ROOM_INCLUDED
#define ROOM2CLOSETS_ROOM_INCLUDED

class room2closets : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room2closets() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room2closets* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
