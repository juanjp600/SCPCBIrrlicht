#ifndef ROOM3_ROOM_INCLUDED
#define ROOM3_ROOM_INCLUDED

class room3 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room3() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room3* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
