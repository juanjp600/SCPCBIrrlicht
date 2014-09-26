#ifndef ROOMPJ_ROOM_INCLUDED
#define ROOMPJ_ROOM_INCLUDED

class roompj : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		roompj() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static roompj* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
