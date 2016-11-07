    /*
    irrDynamics - Light-weight Bullet Physics wrapper for the irrlicht graphics engine
    Copyright (C) 2014  Otto Naderer - otto.naderer@aec.at

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
    */
    //Modified to support trimeshes and filtering
    //Removed global variables

#ifndef _IRRDYNAMICS_H_INCLUDED
#define _IRRDYNAMICS_H_INCLUDED

#include <map>
#include <irrlicht.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

extern btVector3 irrToBtVec(irr::core::vector3df inVec);
extern irr::core::vector3df btToIrrVec(const btVector3 &inVec);

class irrDynamics
{
    public:
		//! Update the physics
		/** This method calculates all the changes in the physics world. Forces are
		  * applied, constraints resolved and collisions treated. It is hence recommendable
		  * to call this method prior to drawing the next frame.
		  * \note If you don't call this method, nothing's gonna happen/move.
		  * \param curTimeStamp The current timestamp in milliseconds. Use ITimer::getRealTime() or anything similar.*/
		void simStep(irr::f32 elapsedTime,float prec=1.f);
		//! Terminate physics and free resources
		/** Once done with simulating physics, this method removes all bullet representations of your scene nodes,
		  * constraints and the entire sim world. Suitable to be called when your application exits.*/
		void shutdown();
		void addTerrain(irr::scene::ITerrainSceneNode* terrain, irr::u32 lodLevel = 2);

		void addTriMesh_static(irr::scene::IMeshSceneNode* mesh,short group=~0,short mask=~0);
		class btRigidBody* addTriMesh_moving(irr::scene::IMeshSceneNode* mesh,float mass=1.0f,unsigned int precisionLoss=1,short group=~0,short mask=~0);

		void registerNewRBody(irr::scene::ISceneNode* node,btRigidBody* rbody,float mass=-1,short group=-1,short mask=~0,irr::core::vector3df nOffset=irr::core::vector3df(0,0,0));
		void unregisterRBody(btRigidBody* rbody);

		//! Add a scene node that is represented as spherical behavior
		/** This method adds an object with ball-like behavior to the physics, such as a SphereSceneNode.
		  * \param node The scene node to be animated by Bullet.
		  * \param radius The radius of the sphere in irrlicht units.
		  * \param mass The mass in units assigned to the object.
		  * \return The corresponding btRigidBody, if addidional parameters are desired to be set. */
		class btRigidBody* addSphericalObject(irr::scene::ISceneNode* node, irr::f32 radius, irr::f32 mass,short group=~0,short mask=~0);
		class btRigidBody* addPlayerColliderObject(irr::scene::ISceneNode* node, irr::f32 height, irr::f32 radius, irr::f32 mass,short group=~0,short mask=~0);

		bool rayTest(const btVector3 &Start,const btVector3 &End);
		irr::core::vector3df rayTestPoint(const btVector3 &Start,const btVector3 &End);
		irr::core::vector3df rayTestNormal(const btVector3 &Start,const btVector3 &End);

		class btRigidBody* addCapsuleObject(irr::scene::ISceneNode* node,irr::f32 height, irr::f32 radius, irr::f32 mass,short group=~0,short mask=~0);
		//! Add a scene node that is represented as a sturdy box
		/** This method adds an object with block-like behavior to the physics, such as a CubeSceneNode.
		  * Its dimensions are calculated using the current axis-aligned bounding box.
		  * \param node The scene node to be animated by Bullet.
		  * \param mass The mass in units assigned to the object.
		  * \return The corresponding btRigidBody, if addidional parameters are desired to be set. */
		class btRigidBody* addBoxObject(irr::scene::ISceneNode* node, irr::f32 mass,short group=~0,short mask=~0);
		//! Add an infinite floor plane to the scene
		/** Whenever an infinite floor catching all your objects is needed, use this method
		  * \param normal The normal vector of the floor plane. Default lays the plane along x- and z-axis (y up).
		  * \param offset The offset from the origin. If normal is default, only the y-value matters here (vert offset).
		  * \return The corresponding btRigidBody, if addidional parameters are desired to be set. */
		class btRigidBody* addFloor(const irr::core::vector3df& normal = irr::core::vector3df(0.f, 1.f, 0.f), const irr::core::vector3df& offset = irr::core::vector3df(0.f, 0.f, 0.f));
		//! Create a hinge relation between two objects
		/** This method physically attaches one scene node to the other in a hinge-like (swinging) relation.
		  * \note Both scene nodes have to be already added to the sim.
		  * \param nodeA The "parent" node the swinging one is attached to.
		  * \param nodeB The child element that will be animated by that relation.
		  * \param pivotInA A position relative to nodeA where the hinge relation will be placed.
		  * \param pivotInB A position relative to nodeB where the hinge relation will be placed.
		  * \param axisInA The axis in nodeA the hinge rotates around.
		  * \param axisInB The axis in nodeB the hinge rotates around.
		  * \return True, when both nodes were found in the physics representation and the relation was created.*/
		bool createHingeConstraint(irr::scene::ISceneNode* nodeA, irr::scene::ISceneNode* nodeB, const irr::core::vector3df& pivotInA, const irr::core::vector3df& pivotInB, const irr::core::vector3df& axisInA, const irr::core::vector3df& axisInB);
		//! Create a point-to-point relation between two objects
		/** More general than a hinge relation, the p2p can freely swing around all axis.
		  * \note Both scene nodes have to be already added to the sim.
		  * \param nodeA The "parent" node the swinging one is attached to.
		  * \param nodeB The child element that will be animated by that relation.
		  * \param pivotInA A position relative to nodeA where the hinge relation will be placed.
		  * \param pivotInB A position relative to nodeB where the hinge relation will be placed.
		  * \return True, when both nodes were found in the physics representation and the relation was created.*/
		bool createPoint2PointConstraint(irr::scene::ISceneNode* nodeA, irr::scene::ISceneNode* nodeB, const irr::core::vector3df& pivotInA, const irr::core::vector3df& pivotInB);
		//! Create a slider relation between two objects
		/** The slider constraint allows a rigid body to rotate around and translate along an axis. This
		  * method uses to the two nodes' current rotation to compute this axis.
		  * \note Both scene nodes have to be already added to the sim.
		  * \param nodeA The "parent" node the constraint is computed from.
		  * \param nodeB The child element that will be animated by that relation.
		  * \return True, when both nodes were found in the physics representation and the relation was created.*/
		bool createSliderConstraint(irr::scene::ISceneNode* nodeA, irr::scene::ISceneNode* nodeB, const irr::core::vector3df& posInA, const irr::core::vector3df& rotInA, const irr::core::vector3df& posInB, const irr::core::vector3df& rotInB);
		//! Remove an object from the physics
		/** Call this whenever you either remove the corresponding scene node from the ISceneManager or you don't want
		  * to have it animated by the physics anymore.
		  * \param node The scene node to be removed from the physics.*/
		void removeObject(irr::scene::ISceneNode* node);
		//! Applies a pushing force to the object
		/** To move an object in a physics engine you do not set its position but you push it towards that place.
		  * Use this method to apply such a push represented as a 3D vector. As expected, the 'longer' the vector,
		  * the more powerful the push will be.
		  * \param node The scene node the force should be applied to.
		  * \param force The force to apply, the vector represents its direction, the vector length its power.
		  * \see setDamping to continuously reduce the effect of applied forces*/
		void applyCentralForce(irr::scene::ISceneNode* node, const irr::core::vector3df& force);
		void applyCentralImpulse(irr::scene::ISceneNode* node, const irr::core::vector3df& force);
		//! Applies a rotational force to the object
		/** Similarly to force/impule application to move an object, use this method to rotate an object.
		  * Set one or more axis values in the provided vector to rotate around the respective axis. As usual
		  * the higher the value in an axis, the more powerful the torque.
		  * \param node The scene node the torque should be applied to.
		  * \param torque The torque to apply, the vector represents the axis to rotate around, the axis value its power.
		  * \see setDamping to continuously reduce the effect of applied torque*/
		void applyTorque(irr::scene::ISceneNode* node, const irr::core::vector3df& torque);
		void setDamping(irr::scene::ISceneNode* node, irr::f32 linearDamping, irr::f32 angularDamping);
		void setPosition(irr::scene::ISceneNode* node, const irr::core::vector3df& newPos);
		//! Change default gravity
		/** Per default, irrDynamics is set to an earth-like gravitational force. Modify that for other planets
		  * or your space sim game.
		  * \param newGravity New gravitational force in m/s^2.*/
		void setGravity(irr::f32 newGravity);

		irrDynamics();

		class btDiscreteDynamicsWorld* getCollisionWorld();

    private:

		irrDynamics(const irrDynamics& other);
		std::map<irr::scene::ISceneNode*, class btRigidBody*> objects;
		std::map<irr::scene::ISceneNode*, irr::core::vector3df*> offset;
		void updateObjects();
		~irrDynamics();
		void clearObjects();
		void removeConstraints(btRigidBody* rigidBody);

		//bt-related stuff
		class btDefaultCollisionConfiguration *collisionConfiguration;
		class btBroadphaseInterface *broadPhase;
		class btCollisionDispatcher *dispatcher;
		class btSequentialImpulseConstraintSolver *solver;
		class btDiscreteDynamicsWorld* world;

		irr::u32 lastStep;
};

/*class dynRegister {
    private:
        irrDynamics* dynamics;
    public:
        void sharedRegisterRBody(irr::scene::ISceneNode* node,btRigidBody* rbody,float mass,short group=~0,short mask=~0,irr::core::vector3df nOffset=irr::core::vector3df(0,0,0));
        void sharedUnregisterRBody(btRigidBody* rbody);

        dynRegister(irrDynamics* dyn);
};*/

#endif
