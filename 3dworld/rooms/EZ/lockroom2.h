#ifndef LOCKROOM2_ROOM_INCLUDED
#define LOCKROOM2_ROOM_INCLUDED

class lockroom2 : public room {
	private:
		static RMesh* baseRMesh;
		lockroom2() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static lockroom2* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
