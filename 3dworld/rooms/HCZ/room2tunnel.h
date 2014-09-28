#ifndef ROOM2TUNNEL_ROOM_INCLUDED
#define ROOM2TUNNEL_ROOM_INCLUDED

class room2tunnel : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room2tunnel() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room2tunnel* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
