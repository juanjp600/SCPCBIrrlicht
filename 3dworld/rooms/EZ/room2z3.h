#ifndef ROOM2Z3_ROOM_INCLUDED
#define ROOM2Z3_ROOM_INCLUDED

class room2z3 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room2z3() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room2z3* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
