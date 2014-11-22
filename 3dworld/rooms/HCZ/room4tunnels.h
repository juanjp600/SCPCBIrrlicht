#ifndef ROOM4TUNNELS_ROOM_INCLUDED
#define ROOM4TUNNELS_ROOM_INCLUDED

class room4tunnels : public room {
	private:
		static RMesh* baseRMesh;
		room4tunnels() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM4; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room4tunnels* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
