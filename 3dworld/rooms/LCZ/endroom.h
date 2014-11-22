#ifndef ENDROOM_ROOM_INCLUDED
#define ENDROOM_ROOM_INCLUDED

class endroom : public room {
	private:
		static RMesh* baseRMesh;
		endroom() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static endroom* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
