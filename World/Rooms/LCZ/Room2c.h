#ifndef ROOM2C_ROOM_INCLUDED
#define ROOM2C_ROOM_INCLUDED

class Room2c : public Room {
	private:
		static RMesh* baseRMesh;
		Room2c() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Room2c* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
