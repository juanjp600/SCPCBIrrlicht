#ifndef ROOM4TUNNELS_ROOM_INCLUDED
#define ROOM4TUNNELS_ROOM_INCLUDED

class Room4tunnels : public Room {
	private:
		static RMesh* baseRMesh;
		Room4tunnels() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM4; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Room4tunnels* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
