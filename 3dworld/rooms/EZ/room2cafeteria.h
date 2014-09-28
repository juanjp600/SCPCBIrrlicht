#ifndef ROOM2CAFETERIA_ROOM_INCLUDED
#define ROOM2CAFETERIA_ROOM_INCLUDED

class room2cafeteria : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room2cafeteria() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room2cafeteria* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
