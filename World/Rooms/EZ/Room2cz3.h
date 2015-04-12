#ifndef ROOM2CZ3_ROOM_INCLUDED
#define ROOM2CZ3_ROOM_INCLUDED

class Room2cz3 : public Room {
	private:
		static RMesh* baseRMesh;
		Room2cz3() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Room2cz3* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
