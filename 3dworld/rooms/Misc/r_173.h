#ifndef R_173_ROOM_INCLUDED
#define R_173_ROOM_INCLUDED

class r_173 : public room {
	private:
		static RMesh* baseRMesh;
		r_173() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static r_173* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
