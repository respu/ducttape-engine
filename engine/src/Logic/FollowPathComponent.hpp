
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_COMPONENT_FOLLOWPATHCOMPONENT
#define DUCTTAPE_ENGINE_COMPONENT_FOLLOWPATHCOMPONENT

#include <Config.hpp>

//#include <Event/Event.hpp>
#include <Scene/Component.hpp>

#include <OgreVector3.h>

#include <QString>

#include <memory>
#include <vector>

namespace dt {

/**
  * Lets the node follow a bezier path.
  * @todo extrude the path stuff to make it usable somewhere else as well
  */
class DUCTTAPE_API FollowPathComponent : public Component {
    Q_OBJECT
public:
    DT_SERIALIZABLE(FollowPathComponent)
    /**
      * The mode which determines what happens when the Node reaches the end of the path.
      */
    enum Mode {
        SINGLE,         //!< Stop the movement at the end.
        LOOP,           //!< Repeat the movement from the beginning.
        ALTERNATING     //!< Move back along the path (forth and back).
    };

    /**
      * Advanced constructor.
      * @see Component
      * @param name The name of the component.
      * @param mode The mode.
      */
    FollowPathComponent(Mode mode = SINGLE, const QString name = "");

    void onInitialize();
    void onDeinitialize();
    void onUpdate(double time_diff);

    /**
      * Adds a point to the end of the path.
      * @param point The new point to add.
      */
    void addPoint(Ogre::Vector3 point);
    void addPoint(float x, float y, float z);

    /**
      * Sets the speed with which the Node should travel along the path.
      * @param speed The speed in units per second.
      */
    void setSpeed(float speed);

    /**
      * Sets the duration the path should take to complete.
      * @param duration The duration in seconds.
      */
    void setDuration(float duration);

    /**
      * Returns the total length of all path segments.
      * @returns The total length of all path segments.
      */
    float getTotalLength() const;

    /**
      * Resets the node to the beginning of the path.
      */
    void reset();

    /**
      * Sets whether the node should accelerate smoothly.
      * @param smooth_acceleration Whether the node should accelerate smoothly.
      */
    void setSmoothAcceleration(bool smooth_acceleration);

    /**
      * Gets whether the node should accelerate smoothly.
      * @returns Whether the node should accelerate smoothly.
      */
    bool getSmoothAcceleration() const;

    /**
      * Sets whether the node should move smoothly around the corners.
      * @param smooth_corners Whether the node should move smoothly around the corners. From 0 (sharp corners) to 1 (no straight edges).
      */
    void setSmoothCorners(bool smooth_corners);

    /**
      * Gets whether the node should move smoothly around the corners.
      * @returns Whether the node should move smoothly around the corners.
      */
    bool getSmoothCorners() const;

    /**
      * Sets whether the node direction should follow the path.
      * @param follow_rotation Whether the node direction should follow the path. From 0 (sharp corners) to 1 (no straight edges).
      */
    void setFollowRotation(bool follow_rotation);

    /**
      * Gets whether the node direction follows the path.
      * @returns Whether the node direction follows the path.
      */
    bool getFollowRotation() const;

    /**
      * Sets the mode.
      * @param mode The mode.
      */
    void setMode(Mode mode);

    /**
      * Returns the mode.
      * @returns The mode.
      */
    Mode getMode() const;

protected:
    /**
      * Calculates the position for the current progress.
      * @returns The position the Node should have right now.
      */
    Ogre::Vector3 _calculatePosition();

private:
    std::vector<Ogre::Vector3> mPoints; //!< The list of path points.
    float mDurationSinceStart;          //!< The time the Node has been traveling.
    float mTotalDuration;               //!< The time it will take the Node to travel the whole path.
    Ogre::Vector3 mLastPoint;           //!< The point the Node has been at in the last frame. Used to calculate the direction.

    bool mSmoothCorners;        //!< Whether the path's corners should be rounded.
    bool mSmoothAcceleration;   //!< Whether the component should accelerate smoothly at the beginning and the end.
    bool mFollowRotation;       //!< Whether the component should rotate while moving along the path to always face the moving direction.
    bool mReversed;             //!< Whether the component is currently on the way back (in ALTERNATING mode).

    Mode mMode;         //!< The movement mode to use.
};

}

#endif
