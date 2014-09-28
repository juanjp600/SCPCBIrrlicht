#ifndef TESTROOM_ROOM_INCLUDED
#define TESTROOM_ROOM_INCLUDED

class testroom : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		testroom() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static testroom* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
