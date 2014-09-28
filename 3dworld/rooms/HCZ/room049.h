#ifndef ROOM049_ROOM_INCLUDED
#define ROOM049_ROOM_INCLUDED

class room049 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room049() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room049* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
