#ifndef TUNNEL_ROOM_INCLUDED
#define TUNNEL_ROOM_INCLUDED

class tunnel : public room {
	private:
		static RMesh* baseRMesh;
		tunnel() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static tunnel* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
