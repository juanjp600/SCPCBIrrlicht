#ifndef ROOM2CLOSETS_ROOM_INCLUDED
#define ROOM2CLOSETS_ROOM_INCLUDED

class room2closets : public room {
	private:
		static RMesh* baseRMesh;
		room2closets() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room2closets* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
