#ifndef ROOM3SERVERS_ROOM_INCLUDED
#define ROOM3SERVERS_ROOM_INCLUDED

class room3servers : public room {
	private:
		static RMesh* baseRMesh;
		room3servers() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static room3servers* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
