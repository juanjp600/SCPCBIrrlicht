#ifndef ROOM860_ROOM_INCLUDED
#define ROOM860_ROOM_INCLUDED

class room860 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room860() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room860* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
