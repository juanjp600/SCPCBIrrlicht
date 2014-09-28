#ifndef START_ROOM_INCLUDED
#define START_ROOM_INCLUDED

class start : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		start() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static start* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
