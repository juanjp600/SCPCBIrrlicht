#ifndef EXIT1_ROOM_INCLUDED
#define EXIT1_ROOM_INCLUDED

class RoomExit1 : public Room {
	private:
		static RMesh* baseRMesh;
		RoomExit1() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static RoomExit1* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
