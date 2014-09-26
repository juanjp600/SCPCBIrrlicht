#ifndef ENDROOM_ROOM_INCLUDED
#define ENDROOM_ROOM_INCLUDED

class endroom : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		endroom() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static endroom* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
