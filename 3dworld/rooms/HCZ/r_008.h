#ifndef R_008_ROOM_INCLUDED
#define R_008_ROOM_INCLUDED

class r_008 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		r_008() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static r_008* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
