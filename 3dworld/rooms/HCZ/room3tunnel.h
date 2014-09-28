#ifndef ROOM3TUNNEL_ROOM_INCLUDED
#define ROOM3TUNNEL_ROOM_INCLUDED

class room3tunnel : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room3tunnel() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room3tunnel* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
