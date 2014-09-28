#ifndef ROOM2OFFICES3_ROOM_INCLUDED
#define ROOM2OFFICES3_ROOM_INCLUDED

class room2offices3 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room2offices3() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room2offices3* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
