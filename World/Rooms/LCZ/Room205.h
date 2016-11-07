#ifndef ROOM205_H_INCLUDED
#define ROOM205_H_INCLUDED

class Room205 : public Room {
	private:
		static RMesh* baseRMesh;
		Room205() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Room205* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif // ROOM205_H_INCLUDED
