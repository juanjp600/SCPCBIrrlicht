#ifndef ROOM2TESTROOM2_ROOM_INCLUDED
#define ROOM2TESTROOM2_ROOM_INCLUDED

class room2testroom2 : public room {
	private:
		static RMesh* baseRMesh;
		room2testroom2() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2testroom2* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
