#ifndef ROOM4Z3_ROOM_INCLUDED
#define ROOM4Z3_ROOM_INCLUDED

class room4z3 : public room {
	private:
		static RMesh* baseRMesh;
		room4z3() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM4; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room4z3* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
