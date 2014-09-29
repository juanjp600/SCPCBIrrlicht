#ifndef ROOM2DOORS_ROOM_INCLUDED
#define ROOM2DOORS_ROOM_INCLUDED

class room2doors : public room {
	private:
		static RMesh* baseRMesh;
		room2doors() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2doors* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif