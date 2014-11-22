#ifndef R_914_ROOM_INCLUDED
#define R_914_ROOM_INCLUDED

class r_914 : public room {
	private:
		static RMesh* baseRMesh;
		r_914() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static r_914* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
