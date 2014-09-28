#ifndef ROOM4_ROOM_INCLUDED
#define ROOM4_ROOM_INCLUDED

class room4 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room4() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM4; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room4* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
