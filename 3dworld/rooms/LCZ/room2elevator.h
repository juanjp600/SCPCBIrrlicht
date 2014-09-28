#ifndef ROOM2ELEVATOR_ROOM_INCLUDED
#define ROOM2ELEVATOR_ROOM_INCLUDED

class room2elevator : public room {
	private:
		static RMesh* baseRMesh;
		room2elevator() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2elevator* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
