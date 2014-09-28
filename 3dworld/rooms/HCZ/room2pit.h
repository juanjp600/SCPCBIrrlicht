#ifndef ROOM2PIT_ROOM_INCLUDED
#define ROOM2PIT_ROOM_INCLUDED

class room2pit : public room {
	private:
		static RMesh* baseRMesh;
		room2pit() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2pit* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
