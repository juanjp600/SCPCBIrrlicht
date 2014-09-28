#ifndef ROOM2CTUNNEL_ROOM_INCLUDED
#define ROOM2CTUNNEL_ROOM_INCLUDED

class room2ctunnel : public room {
	private:
		static RMesh* baseRMesh;
		room2ctunnel() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2ctunnel* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
