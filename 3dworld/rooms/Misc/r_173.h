#ifndef R_173_ROOM_INCLUDED
#define R_173_ROOM_INCLUDED

class r_173 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		r_173() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static r_173* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
