// Custom level class to load a level from a PropertyList.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------

#include "CustomLevel.h"

#include <Scene/SceneNode.h>
#include <Utils/PropertyList.h>

//Resources
#include <Resources/ResourceManager.h>
#include <Resources/TGAResource.h>
#include <Resources/GLSLResource.h>
#include <Resources/OBJResource.h>

//Transformers
#include <Scene/CollectedGeometryTransformer.h>
#include <Scene/QuadTransformer.h>
#include <Scene/BSPTransformer.h>

//#include "Serializer.h"

namespace OpenEngine {

namespace Level {

using OpenEngine::Scene::ISceneNode;
using OpenEngine::Utils::PropertyList;
using namespace OpenEngine::Resources;
    using namespace OpenEngine::Scene;



CustomLevel::CustomLevel(PropertyList& p): p(p) {
    scene = NULL;
    physics = NULL;
}

CustomLevel::~CustomLevel() {
    if (scene != NULL ||
        physics != NULL)
        Unload();
}

void CustomLevel::Load() {
    // First unload level if already loaded
    if (scene != NULL ||
        physics != NULL)
        Unload();
    
    // TODO: create exception to tell if lvl file is invalid
    
    bool createBSP = true;

    physics = new SceneNode();// = *physics;
    scene = new SceneNode();

//     if (p.HaveKey("bspfile") && 
//         Deserialize(*physics, p.GetString("bspfile").data())) {
//         //physics = physN;
//         createBSP = false;
//     }
    
    IModelResourcePtr res;
    ISceneNode* geom;

//    FaceSet* fs;


    for (int i = 0; i < p.ListSize("sGeometry"); i++) {
        res = ResourceManager<IModelResource>::Create(p.GetString("sGeometry", i).data());
		res->Load();
        geom = res->GetSceneNode();
        scene->AddNode(geom);
        
        if (createBSP) {
            physics->AddNode(geom->Clone());
        }
    }


    CollectedGeometryTransformer geoTrans = CollectedGeometryTransformer();
    QuadTransformer quadTrans = QuadTransformer();

    geoTrans.Transform(*scene);
    quadTrans.SetMaxFaceCount(500);
    quadTrans.Transform(*scene);

    for (int i = 0; i < p.ListSize("nsGeometry"); i++) {
        res = ResourceManager<IModelResource>::Create(p.GetString("nsGeometry", i).data());
		res->Load();
        geom = res->GetSceneNode();
        scene->AddNode(geom);
    }

   
    if (createBSP) {
        BSPTransformer bspTrans = BSPTransformer();

        // Transform the part of the scene that should have physics into
        // one geoNode.
        geoTrans.Transform(*physics);
        
        // Transform the geoNode to a quadTree
        //quadTrans.SetMaxFaceCount(100);
        //quadTrans.Transform(*physics);
        
        // Gives the quadTree BSP leaf nodes.
        bspTrans.Transform(*physics);
       
        if (p.HaveKey("bspfile")) {
            //Serialize(*physics, p.GetString("bspfile").data());
        }
    }
}
    

void CustomLevel::Unload() {
    
}

SceneNode* CustomLevel::GetSceneGraph() {
    return scene;
}

SceneNode* CustomLevel::GetPhysicsGraph() {
    return physics;
}


} // NS Level
} // NS OpenEngine
