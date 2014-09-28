#ifndef GATEAENTRANCE_ROOM_INCLUDED
#define GATEAENTRANCE_ROOM_INCLUDED

class gateaentrance : public room {
	private:
		static RMesh* baseRMesh;
		gateaentrance() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static gateaentrance* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
