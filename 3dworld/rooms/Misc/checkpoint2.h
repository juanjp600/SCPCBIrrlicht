#ifndef CHECKPOINT2_ROOM_INCLUDED
#define CHECKPOINT2_ROOM_INCLUDED

class checkpoint2 : public room {
	private:
		static RMesh* baseRMesh;
		checkpoint2() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static checkpoint2* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
