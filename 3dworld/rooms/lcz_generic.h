#ifndef LCZ_GENERIC_H_INCLUDED
#define LCZ_GENERIC_H_INCLUDED

//generic, featureless rooms for the Light Containment Zone

class room1 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room1() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; };
		virtual void updateEvent() { return; };
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room1* createNew(irr::core::vector3df inPosition,char inAngle);
};

class room2 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room2() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; };
		virtual void updateEvent() { return; };
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room2* createNew(irr::core::vector3df inPosition,char inAngle);
};

class room2c : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room2c() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2C; };
		virtual void updateEvent() { return; };
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room2c* createNew(irr::core::vector3df inPosition,char inAngle);
};

class room3 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room3() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM3; };
		virtual void updateEvent() { return; };
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room3* createNew(irr::core::vector3df inPosition,char inAngle);
};

class room4 : public room {
	private:
		static irr::scene::IMeshSceneNode* baseNode;
		static btBvhTriangleMeshShape* baseShape;
		room4() {};
	public:
		virtual roomTypes getType() { return roomTypes::ROOM4; };
		virtual void updateEvent() { return; };
		static void setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape);
		static room4* createNew(irr::core::vector3df inPosition,char inAngle);
};

#endif // LCZ_GENERIC_H_INCLUDED
