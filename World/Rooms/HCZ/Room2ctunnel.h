#ifndef ROOM2CTUNNEL_ROOM_INCLUDED
#define ROOM2CTUNNEL_ROOM_INCLUDED

class Room2ctunnel : public Room {
	private:
		static RMesh* baseRMesh;
		Room2ctunnel() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Room2ctunnel* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
