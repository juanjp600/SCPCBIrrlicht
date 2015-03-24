#ifndef ROOM079_ROOM_INCLUDED
#define ROOM079_ROOM_INCLUDED

class room079 : public room {
	private:
		static RMesh* baseRMesh;
		room079() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static room079* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
