#ifndef ROOM2NUKE_ROOM_INCLUDED
#define ROOM2NUKE_ROOM_INCLUDED

class room2nuke : public room {
	private:
		static RMesh* baseRMesh;
		room2nuke() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2nuke* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
