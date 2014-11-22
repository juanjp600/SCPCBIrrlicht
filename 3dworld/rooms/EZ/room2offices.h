#ifndef ROOM2OFFICES_ROOM_INCLUDED
#define ROOM2OFFICES_ROOM_INCLUDED

class room2offices : public room {
	private:
		static RMesh* baseRMesh;
		room2offices() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2offices* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
