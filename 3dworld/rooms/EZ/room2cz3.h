#ifndef ROOM2CZ3_ROOM_INCLUDED
#define ROOM2CZ3_ROOM_INCLUDED

class room2cz3 : public room {
	private:
		static RMesh* baseRMesh;
		room2cz3() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static room2cz3* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
