#ifndef ROOM3SERVERS_ROOM_INCLUDED
#define ROOM3SERVERS_ROOM_INCLUDED

class Room3servers : public Room {
	private:
		static RMesh* baseRMesh;
		Room3servers() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Room3servers* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
