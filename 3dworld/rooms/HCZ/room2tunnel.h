#ifndef ROOM2TUNNEL_ROOM_INCLUDED
#define ROOM2TUNNEL_ROOM_INCLUDED

class room2tunnel : public room {
	private:
		static RMesh* baseRMesh;
		room2tunnel() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2tunnel* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif