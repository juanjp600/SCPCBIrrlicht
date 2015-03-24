#ifndef R_008_ROOM_INCLUDED
#define R_008_ROOM_INCLUDED

class r_008 : public room {
	private:
		static RMesh* baseRMesh;
		r_008() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static r_008* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
