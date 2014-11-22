#ifndef ROOM3SERVERS_ROOM_INCLUDED
#define ROOM3SERVERS_ROOM_INCLUDED

class room3servers : public room {
	private:
		static RMesh* baseRMesh;
		room3servers() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room3servers* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
