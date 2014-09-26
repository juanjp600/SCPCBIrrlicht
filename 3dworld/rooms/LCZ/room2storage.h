#ifndef ROOM2STORAGE_ROOM_INCLUDED
#define ROOM2STORAGE_ROOM_INCLUDED

class room2storage : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room2storage() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room2storage* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
