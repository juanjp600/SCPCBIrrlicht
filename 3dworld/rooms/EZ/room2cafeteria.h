#ifndef ROOM2CAFETERIA_ROOM_INCLUDED
#define ROOM2CAFETERIA_ROOM_INCLUDED

class room2cafeteria : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room2cafeteria() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room2cafeteria* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
