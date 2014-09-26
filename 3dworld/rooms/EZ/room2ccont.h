#ifndef ROOM2CCONT_ROOM_INCLUDED
#define ROOM2CCONT_ROOM_INCLUDED

class room2ccont : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room2ccont() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room2ccont* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
