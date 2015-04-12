#ifndef ROOMPJ_ROOM_INCLUDED
#define ROOMPJ_ROOM_INCLUDED

class Roompj : public Room {
	private:
		static RMesh* baseRMesh;
		Roompj() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Roompj* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
