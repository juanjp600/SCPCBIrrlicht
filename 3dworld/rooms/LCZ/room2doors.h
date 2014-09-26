#ifndef ROOM2DOORS_ROOM_INCLUDED
#define ROOM2DOORS_ROOM_INCLUDED

class room2doors : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room2doors() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room2doors* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
