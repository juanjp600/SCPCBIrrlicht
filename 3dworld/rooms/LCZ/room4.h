#ifndef ROOM4_ROOM_INCLUDED
#define ROOM4_ROOM_INCLUDED

class room4 : public room {
	private:
		static RMesh* baseRMesh;
		room4() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM4; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static room4* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
