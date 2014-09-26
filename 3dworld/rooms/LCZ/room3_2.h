#ifndef ROOM3_2_ROOM_INCLUDED
#define ROOM3_2_ROOM_INCLUDED

class room3_2 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room3_2() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room3_2* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
