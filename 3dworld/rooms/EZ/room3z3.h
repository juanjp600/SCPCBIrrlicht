#ifndef ROOM3Z3_ROOM_INCLUDED
#define ROOM3Z3_ROOM_INCLUDED

class room3z3 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room3z3() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room3z3* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
