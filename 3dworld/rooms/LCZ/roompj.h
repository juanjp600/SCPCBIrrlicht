#ifndef ROOMPJ_ROOM_INCLUDED
#define ROOMPJ_ROOM_INCLUDED

class roompj : public room {
	private:
		static RMesh* baseRMesh;
		roompj() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static roompj* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
