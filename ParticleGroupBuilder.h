/*
 *  ParticleGroupBuilder.h
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 02/05/08.
 *  Copyright 2008 Lucky Software. All rights reserved.
 *
 */

#ifndef _PATSYS_PARTICLE_GROUP_BUILDER_H_
#define _PATSYS_PARTICLE_GROUP_BUILDER_H_

#include <Particles/ParticleGroup.h>
#include <Utils/PropertyList.h>
#include <Renderers/IRenderNode.h>

using namespace OpenEngine::Particles;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Renderers;

class ParticleGroupBuilder {
    PropertyList& plist;
    IParticleGroup* pGroup;
    IRenderNode* rNode;
    string group;
public:
    ParticleGroupBuilder(PropertyList& plist, string group);
    
    IParticleGroup* GetParticleGroup();
    IRenderNode* GetRenderNode();
    
    static pair<IParticleGroup*,IRenderNode*> BuildGroup(PropertyList& plist, string group);
    template <class T> static IEmitter<T>* BuildEmitter(PropertyList& plist, string group);
    template <class T> static T* BuildParticle(PropertyList& plist, string group);
    template <class T, class G> static IRenderNode* BuildRenderNode(PropertyList& plist, string group, G* g);
    template <class T> static IModifier<T>* BuildModifier(PropertyList& plist, string group);
};

#endif