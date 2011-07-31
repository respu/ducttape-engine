
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_UTILS_STRINGMANAGER
#define DUCTTAPE_ENGINE_UTILS_STRINGMANAGER

#include <cstdint>
#include <string>
#include <map>

#include "Config.hpp"
#include "Manager.hpp"

namespace dt {

/**
  * A class used to translate strings to global Ids and back. Used in server and client applications. The Ids are required to keep
  * the network traffic to a minimum (a uint32_t is smaller than most strings), but strings are used to keep the source code clean
  * and readable and to output (debug) messages in a readable format.
  */
class DUCTTAPE_API StringManager : public Manager {
public:
    /**
      * Default constructor.
      */
    StringManager();

    void Initialize();
    void Deinitialize();

    /**
      * Returns a pointer to the Manager instance.
      * @returns A pointer to the Manager instance.
      */
    static StringManager* Get();

    /**
      * Registers a new string with a generated Id.
      * @param string The string to register.
      * @returns The new Id.
      */
    uint32_t Add(const std::string& string);

    /**
      * Checks whether a string is already registered.
      * @param string The string to be checked.
      * @returns true If the string is already registered, otherwise false.
      */
    bool Has(const std::string& string);

    /**
      * Checks whether an Id is already used.
      * @param id The Id to be checked.
      * @returns true If the Id is already used, otherwise false.
      */
    bool Has(uint32_t id);

    /**
      * Returns the Id for a string.
      * @param string The string to find.
      * @returns The Id for the string.
      */
    uint32_t GetId(const std::string& string);

    /**
      * Returns the string for an Id.
      * @param id The Id to find.
      * @returns The string for the Id.
      */
    const std::string& GetString(uint32_t id);

    /**
      * Returns next Id for automatic name generation that is available.
      * @returns The uint32_t with next available Id for automatic name generation.
      */
    uint32_t GetNextAutoId();

private:
    uint32_t mLastId;                       //!< The Id used to register the last string with.
    uint32_t mAutoId;                       //!< The Id used for the last automatic name generation.
    std::map<uint32_t, std::string> mIds;   //!< The relation map between Ids/strings.
};

}

#endif
