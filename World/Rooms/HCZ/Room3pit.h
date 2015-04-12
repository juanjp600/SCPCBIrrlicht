#ifndef ROOM3PIT_ROOM_INCLUDED
#define ROOM3PIT_ROOM_INCLUDED

class Room3pit : public Room {
	private:
		static RMesh* baseRMesh;
		Room3pit() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Room3pit* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
