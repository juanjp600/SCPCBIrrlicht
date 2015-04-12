#ifndef TUNNEL_ROOM_INCLUDED
#define TUNNEL_ROOM_INCLUDED

class RoomTunnel : public Room {
	private:
		static RMesh* baseRMesh;
		RoomTunnel() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static RoomTunnel* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
