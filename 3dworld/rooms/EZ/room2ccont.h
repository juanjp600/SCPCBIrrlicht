#ifndef ROOM2CCONT_ROOM_INCLUDED
#define ROOM2CCONT_ROOM_INCLUDED

class room2ccont : public room {
	private:
		static RMesh* baseRMesh;
		room2ccont() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2ccont* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
