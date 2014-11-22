#ifndef START_ROOM_INCLUDED
#define START_ROOM_INCLUDED

class start : public room {
	private:
		static RMesh* baseRMesh;
		start() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static start* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
