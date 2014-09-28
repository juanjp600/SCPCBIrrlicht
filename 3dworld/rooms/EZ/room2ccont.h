#ifndef ROOM2CCONT_ROOM_INCLUDED
#define ROOM2CCONT_ROOM_INCLUDED

class room2ccont : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room2ccont() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room2ccont* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
