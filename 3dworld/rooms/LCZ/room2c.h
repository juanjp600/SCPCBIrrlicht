#ifndef ROOM2C_ROOM_INCLUDED
#define ROOM2C_ROOM_INCLUDED

class room2c : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room2c() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room2c* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
