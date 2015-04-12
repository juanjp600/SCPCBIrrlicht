#ifndef GATEA_ROOM_INCLUDED
#define GATEA_ROOM_INCLUDED

class RoomGatea : public Room {
	private:
		static RMesh* baseRMesh;
		RoomGatea() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static RoomGatea* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
