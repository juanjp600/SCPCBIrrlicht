#ifndef ROOM2_2_ROOM_INCLUDED
#define ROOM2_2_ROOM_INCLUDED

class room2_2 : public room {
	private:
		static RMesh* baseRMesh;
		room2_2() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2_2* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
