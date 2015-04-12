#ifndef CHECKPOINT1_ROOM_INCLUDED
#define CHECKPOINT1_ROOM_INCLUDED

class RoomCheckpoint1 : public Room {
	private:
		static RMesh* baseRMesh;
		RoomCheckpoint1() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static RoomCheckpoint1* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
