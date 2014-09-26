#ifndef EXIT1_ROOM_INCLUDED
#define EXIT1_ROOM_INCLUDED

class exit1 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		exit1() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static exit1* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
