#ifndef ROOM2TUNNEL_ROOM_INCLUDED
#define ROOM2TUNNEL_ROOM_INCLUDED

class room2tunnel : public room {
	private:
		static RMesh* baseRMesh;
		room2tunnel() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static room2tunnel* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
