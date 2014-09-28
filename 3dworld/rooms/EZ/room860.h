#ifndef ROOM860_ROOM_INCLUDED
#define ROOM860_ROOM_INCLUDED

class room860 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static RMesh* baseRMesh;
		room860() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(irr::scene::IMeshSceneNode* inNode,RMesh* inRme);
		static room860* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
