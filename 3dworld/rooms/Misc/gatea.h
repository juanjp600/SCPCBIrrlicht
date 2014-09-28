#ifndef GATEA_ROOM_INCLUDED
#define GATEA_ROOM_INCLUDED

class gatea : public room {
	private:
		static RMesh* baseRMesh;
		gatea() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static gatea* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
