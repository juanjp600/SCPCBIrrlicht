#ifndef ENDROOM2_ROOM_INCLUDED
#define ENDROOM2_ROOM_INCLUDED

class endroom2 : public room {
	private:
		static RMesh* baseRMesh;
		endroom2() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static endroom2* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
