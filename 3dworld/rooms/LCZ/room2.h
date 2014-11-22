#ifndef ROOM2_ROOM_INCLUDED
#define ROOM2_ROOM_INCLUDED

class room2 : public room {
	private:
		static RMesh* baseRMesh;
		room2() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
