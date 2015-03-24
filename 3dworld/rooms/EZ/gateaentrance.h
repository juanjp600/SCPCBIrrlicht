#ifndef GATEAENTRANCE_ROOM_INCLUDED
#define GATEAENTRANCE_ROOM_INCLUDED

class gateaentrance : public room {
	private:
		static RMesh* baseRMesh;
		gateaentrance() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static gateaentrance* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
