// Level Interface.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------
#ifndef _CGD_ILEVEL_H_
#define _CGD_ILEVEL_H_

namespace OpenEngine {
    //forward declarations
    namespace Scene { 
        class SceneNode; 
    }

namespace Level {

using OpenEngine::Scene::SceneNode;

class ILevel {
    ILevel() {}
    virtual ~ILevel() {}

    virtual void Load() = 0;
    virtual void Unload() = 0;
    virtual SceneNode* GetSceneGraph() = 0;
    virtual SceneNode* GetPhysicsGraph() = 0;
};

} // NS Level
} // NS OpenEngine

#endif //_CGD_ILEVEL_H_
