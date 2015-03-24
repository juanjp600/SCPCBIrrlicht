#ifndef TUNNEL2_ROOM_INCLUDED
#define TUNNEL2_ROOM_INCLUDED

class tunnel2 : public room {
	private:
		static RMesh* baseRMesh;
		tunnel2() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static tunnel2* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
