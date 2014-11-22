#ifndef COFFIN_ROOM_INCLUDED
#define COFFIN_ROOM_INCLUDED

class coffin : public room {
	private:
		static RMesh* baseRMesh;
		coffin() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static coffin* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
