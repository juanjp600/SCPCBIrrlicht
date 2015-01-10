#ifndef ROOM513_ROOM_INCLUDED
#define ROOM513_ROOM_INCLUDED

class room513 : public room {
	private:
		static RMesh* baseRMesh;
		room513() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room513* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
