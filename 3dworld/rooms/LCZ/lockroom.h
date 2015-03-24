#ifndef LOCKROOM_ROOM_INCLUDED
#define LOCKROOM_ROOM_INCLUDED

class lockroom : public room {
	private:
		static RMesh* baseRMesh;
		lockroom() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static lockroom* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
