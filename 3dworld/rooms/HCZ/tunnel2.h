#ifndef TUNNEL2_ROOM_INCLUDED
#define TUNNEL2_ROOM_INCLUDED

class tunnel2 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		tunnel2() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static tunnel2* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
