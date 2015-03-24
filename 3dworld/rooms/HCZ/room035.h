#ifndef ROOM035_ROOM_INCLUDED
#define ROOM035_ROOM_INCLUDED

class room035 : public room {
	private:
		static RMesh* baseRMesh;
		room035() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static room035* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
