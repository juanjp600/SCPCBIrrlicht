#ifndef LOCKROOM_ROOM_INCLUDED
#define LOCKROOM_ROOM_INCLUDED

class lockroom : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		lockroom() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static lockroom* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
