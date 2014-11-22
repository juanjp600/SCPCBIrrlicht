#ifndef ROOM3SERVERS2_ROOM_INCLUDED
#define ROOM3SERVERS2_ROOM_INCLUDED

class room3servers2 : public room {
	private:
		static RMesh* baseRMesh;
		room3servers2() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room3servers2* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
