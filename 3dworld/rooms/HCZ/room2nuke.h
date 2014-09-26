#ifndef ROOM2NUKE_ROOM_INCLUDED
#define ROOM2NUKE_ROOM_INCLUDED

class room2nuke : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room2nuke() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room2nuke* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
