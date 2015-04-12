#ifndef ROOM3_2_ROOM_INCLUDED
#define ROOM3_2_ROOM_INCLUDED

class Room3_2 : public Room {
	private:
		static RMesh* baseRMesh;
		Room3_2() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Room3_2* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
