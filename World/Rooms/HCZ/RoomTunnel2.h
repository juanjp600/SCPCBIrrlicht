#ifndef TUNNEL2_ROOM_INCLUDED
#define TUNNEL2_ROOM_INCLUDED

class RoomTunnel2 : public Room {
	private:
		static RMesh* baseRMesh;
		RoomTunnel2() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static RoomTunnel2* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
