#ifndef CHECKPOINT1_ROOM_INCLUDED
#define CHECKPOINT1_ROOM_INCLUDED

class checkpoint1 : public room {
	private:
		static RMesh* baseRMesh;
		checkpoint1() {};
	public:
		virtual roomTypes getType() const { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static checkpoint1* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
