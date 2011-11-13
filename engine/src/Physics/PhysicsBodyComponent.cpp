
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Physics/PhysicsBodyComponent.hpp>

#include <Graphics/MeshComponent.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

#include <BtOgreGP.h>

namespace dt {

PhysicsBodyComponent::PhysicsBodyComponent(const QString& mesh_component_name,
                                           const QString& name)
    : Component(name),
      mMeshComponentName(mesh_component_name),
      mCollisionShape(nullptr),
      mBody(nullptr),
      mMotionState(nullptr),
      mCentralForce(btVector3(0, 0, 0)),
      mTorque(btVector3(0, 0, 0)) {}

void PhysicsBodyComponent::OnCreate() {
    if(! mNode->HasComponent(mMeshComponentName)) {
        Logger::Get().Error("Node "+mNode->GetName()+" has no Component named "+
                            mMeshComponentName+" which is required to create the"+
                            " PhysicsBodyComponent "+mName);
        exit(1);
    }

    MeshComponent* mesh_component =
        mNode->FindComponent<MeshComponent>(mMeshComponentName);

    BtOgre::StaticMeshToShapeConverter converter(mesh_component->GetOgreEntity());

    // TODO: CollisionShapes should likely be stored at a central place.
    // Perhaps the PhysicsManager is a good place. It would save a lot of memory
    // for many bodies with the same CollisionShape.
    mCollisionShape = converter.createConvex();

    btScalar mass = 5;
    btVector3 inertia;
    mCollisionShape->calculateLocalInertia(mass, inertia);

    btDefaultMotionState* state = new btDefaultMotionState(
        btTransform(BtOgre::Convert::toBullet(GetNode()->GetRotation(Node::SCENE)),
                    BtOgre::Convert::toBullet(GetNode()->GetPosition(Node::SCENE))));

    mBody = new btRigidBody(mass, state, mCollisionShape, inertia);

    // Store pointer to this PhysicsBodyComponent for later retrieval (for
    // collisions, for instance)
    mBody->setUserPointer((void *)(this));
}

void PhysicsBodyComponent::SetCentralForce(const btVector3& force) {
    mCentralForce = force;
}

void PhysicsBodyComponent::SetTorque(const btVector3& torque)
{
    mTorque = torque;
}

void PhysicsBodyComponent::OnDestroy() {
    delete mBody->getMotionState();
    delete mBody;
    delete mCollisionShape;
}

void PhysicsBodyComponent::OnEnable() {
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->addRigidBody(mBody);
}

void PhysicsBodyComponent::OnDisable() {
    GetNode()->GetScene()->GetPhysicsWorld()->GetBulletWorld()->removeRigidBody(mBody);
}

const btVector3 PhysicsBodyComponent::GetCentralForce() const {
    return mCentralForce;
}

const btVector3 PhysicsBodyComponent::GetTorque() const {
    return mTorque;
}

void PhysicsBodyComponent::SetGravity(const btVector3& gravity) {
    mBody->setGravity(gravity);
}

void PhysicsBodyComponent::SetTwoDimensional(bool twod) {
    if(twod)
    {
        mBody->setLinearFactor(btVector3(1, 1, 0));
        mBody->setAngularFactor(btVector3(0, 0, 1));
    }
}

void PhysicsBodyComponent::DisableDeactivation(bool disabled) {
    if(disabled) {
        mBody->setActivationState(DISABLE_DEACTIVATION);
    }
}

void PhysicsBodyComponent::SetDampingAmount(btScalar lindamping, btScalar angdamping) {
    mBody->setDamping(lindamping, angdamping);
}

void PhysicsBodyComponent::OnUpdate(double time_diff) {
    btTransform trans;
    mBody->getMotionState()->getWorldTransform(trans);
    /*std::cout << "x: " << trans.getOrigin().getX() << " y: "
        << trans.getOrigin().getY() << " z: " << trans.getOrigin().getZ() << std::endl;*/

    if(!mCentralForce.isZero()) {
        mBody->applyCentralForce(mCentralForce);
    }

    if(!mTorque.isZero()) {
        mBody->applyTorque(mTorque);
    }

    GetNode()->SetPosition(BtOgre::Convert::toOgre(trans.getOrigin()), Node::SCENE);
    GetNode()->SetRotation(BtOgre::Convert::toOgre(trans.getRotation()), Node::SCENE);
}

void PhysicsBodyComponent::SetMass(btScalar mass) {
    btVector3 inertia;
    mCollisionShape->calculateLocalInertia(mass, inertia);
    mBody->setMassProps(mass, inertia);
}

}
