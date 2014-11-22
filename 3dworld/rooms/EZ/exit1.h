#ifndef EXIT1_ROOM_INCLUDED
#define EXIT1_ROOM_INCLUDED

class exit1 : public room {
	private:
		static RMesh* baseRMesh;
		exit1() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static exit1* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
