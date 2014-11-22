#ifndef R_008_ROOM_INCLUDED
#define R_008_ROOM_INCLUDED

class r_008 : public room {
	private:
		static RMesh* baseRMesh;
		r_008() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static r_008* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
