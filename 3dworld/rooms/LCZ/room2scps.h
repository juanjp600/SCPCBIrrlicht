#ifndef ROOM2SCPS_ROOM_INCLUDED
#define ROOM2SCPS_ROOM_INCLUDED

class room2scps : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room2scps() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room2scps* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
