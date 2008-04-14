// Custom level class to load a level from a PropertyList.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------

#ifndef _CGD_CUSTOM_LEVEL_H_
#define _CGD_CUSTOM_LEVEL_H_


#include "ILevel.h"

namespace OpenEngine {
    //forward declarations
    namespace Scene { 
        class SceneNode; 
    }
    namespace Utils {
        class PropertyList;
    }

namespace Level {

using OpenEngine::Scene::SceneNode;
using OpenEngine::Utils::PropertyList;

class CustomLevel {
private:
    PropertyList& p;
    SceneNode* scene;
    SceneNode* physics;
public:
    CustomLevel(PropertyList& p);
    ~CustomLevel();

    void Load();
    void Unload();
    SceneNode* GetSceneGraph();
    SceneNode* GetPhysicsGraph();
};

} // NS Level
} // NS OpenEngine

#endif //_CGD_CUSTOM_LEVEL_H_
