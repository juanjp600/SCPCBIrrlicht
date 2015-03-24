#ifndef ROOM3STORAGE_ROOM_INCLUDED
#define ROOM3STORAGE_ROOM_INCLUDED

class room3storage : public room {
	private:
		static RMesh* baseRMesh;
		room3storage() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static room3storage* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
