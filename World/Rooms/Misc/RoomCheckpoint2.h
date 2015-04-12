#ifndef CHECKPOINT2_ROOM_INCLUDED
#define CHECKPOINT2_ROOM_INCLUDED

class RoomCheckpoint2 : public Room {
	private:
		static RMesh* baseRMesh;
		RoomCheckpoint2() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static RoomCheckpoint2* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
