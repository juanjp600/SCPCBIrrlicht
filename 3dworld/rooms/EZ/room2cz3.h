#ifndef ROOM2CZ3_ROOM_INCLUDED
#define ROOM2CZ3_ROOM_INCLUDED

class room2cz3 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room2cz3() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room2cz3* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
