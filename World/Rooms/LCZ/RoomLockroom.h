#ifndef LOCKROOM_ROOM_INCLUDED
#define LOCKROOM_ROOM_INCLUDED

class RoomLockroom : public Room {
	private:
		static RMesh* baseRMesh;
		RoomLockroom() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static RoomLockroom* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
