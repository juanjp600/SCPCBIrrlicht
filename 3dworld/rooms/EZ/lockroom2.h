#ifndef LOCKROOM2_ROOM_INCLUDED
#define LOCKROOM2_ROOM_INCLUDED

class lockroom2 : public room {
	private:
		static RMesh* baseRMesh;
		lockroom2() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static lockroom2* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
