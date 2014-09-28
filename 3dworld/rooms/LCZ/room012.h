#ifndef ROOM012_ROOM_INCLUDED
#define ROOM012_ROOM_INCLUDED

class room012 : public room {
	private:
		static RMesh* baseRMesh;
		room012() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		virtual const std::vector<irr::video::SLight>& getPointLights();
		static void setBase(RMesh* inRme);
		static room012* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif
