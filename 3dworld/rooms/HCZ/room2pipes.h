#ifndef ROOM2PIPES_ROOM_INCLUDED
#define ROOM2PIPES_ROOM_INCLUDED

class room2pipes : public room {
	private:
		static RMesh* baseRMesh;
		room2pipes() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2pipes* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif