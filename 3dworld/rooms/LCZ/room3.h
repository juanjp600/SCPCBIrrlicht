#ifndef ROOM3_ROOM_INCLUDED
#define ROOM3_ROOM_INCLUDED

class room3 : public room {
	private:
		static RMesh* baseRMesh;
		room3() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM3; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room3* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
