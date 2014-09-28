#ifndef ROOM2ELEVATOR_ROOM_INCLUDED
#define ROOM2ELEVATOR_ROOM_INCLUDED

class room2elevator : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room2elevator() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room2elevator* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
