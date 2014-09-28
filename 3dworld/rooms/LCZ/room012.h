#ifndef ROOM012_ROOM_INCLUDED
#define ROOM012_ROOM_INCLUDED

class room012 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room012() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room012* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
