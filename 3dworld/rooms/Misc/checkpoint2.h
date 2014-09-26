#ifndef CHECKPOINT2_ROOM_INCLUDED
#define CHECKPOINT2_ROOM_INCLUDED

class checkpoint2 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		checkpoint2() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static checkpoint2* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
