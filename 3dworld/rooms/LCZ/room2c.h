#ifndef ROOM2C_ROOM_INCLUDED
#define ROOM2C_ROOM_INCLUDED

class room2c : public room {
	private:
		static RMesh* baseRMesh;
		room2c() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2C; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2c* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
