#ifndef ROOM2SROOM_ROOM_INCLUDED
#define ROOM2SROOM_ROOM_INCLUDED

class room2sroom : public room {
	private:
		static RMesh* baseRMesh;
		room2sroom() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2sroom* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
