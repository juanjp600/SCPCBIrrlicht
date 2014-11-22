#ifndef ENDROOM2_ROOM_INCLUDED
#define ENDROOM2_ROOM_INCLUDED

class endroom2 : public room {
	private:
		static RMesh* baseRMesh;
		endroom2() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static endroom2* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
