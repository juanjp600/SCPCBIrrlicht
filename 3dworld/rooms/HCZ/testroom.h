#ifndef TESTROOM_ROOM_INCLUDED
#define TESTROOM_ROOM_INCLUDED

class testroom : public room {
	private:
		static RMesh* baseRMesh;
		testroom() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static testroom* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
