#ifndef ROOM1123_H_INCLUDED
#define ROOM1123_H_INCLUDED

class Room1123 : public Room {
	private:
		static RMesh* baseRMesh;
		Room1123() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Room1123* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif // ROOM1123_H_INCLUDED
