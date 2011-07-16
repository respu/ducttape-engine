
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_EVENT_BEGINFRAMEEVENT
#define DUCTTAPE_ENGINE_EVENT_BEGINFRAMEEVENT

#include "Event.hpp"

namespace dt {

/**
  * Event that is being triggered at the beginning of each frame, carrying the frame time delta.
  */
class BeginFrameEvent : public Event {
public:
    /**
      * Advanced constructor.
      * @param frame_time The time delta of the current frame.
      */
    BeginFrameEvent(uint32_t frame_time);
    const std::string GetType() const;
    Event* Clone() const;


    /**
      * Returns the time delta of the current frame.
      * @returns The time delta of the current frame.
      */
    uint32_t GetFrameTime() const;

private:
    uint32_t mFrameTime;       //!< The time delta of the current frame.

};

}

#endif
