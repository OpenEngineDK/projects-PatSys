/*
 *  ParticleGroupBuilder.cpp
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 02/05/08.
 *  Copyright 2008 Lucky Software. All rights reserved.
 *
 */

#include "ParticleGroupBuilder.h"
#include <Particles/Particles.h>
#include <Particles/Modifiers.h>
#include <Particles/ParticleGroups.h>
#include <Logging/Logger.h>

#define ParticleTypeGlow BillBoardParticle<EnergyParticle<DirectionParticle<IParticle> > >

using namespace OpenEngine::Particles;

ParticleGroupBuilder::ParticleGroupBuilder(PropertyList& plist, string group) :plist(plist), group(group) {
    pair<IParticleGroup*,IRenderNode*> p = BuildGroup(plist, group);
    pGroup = p.first;
    rNode = p.second;
}

IParticleGroup* ParticleGroupBuilder::GetParticleGroup() {
    return pGroup;
}

IRenderNode* ParticleGroupBuilder::GetRenderNode() {
    return rNode;
}



// static

pair<IParticleGroup*,IRenderNode*> ParticleGroupBuilder::BuildGroup(PropertyList& plist, string group) {
    string type = plist.GetString(group + ".type");
    if (type == "glow") {
        
        IEmitter<ParticleTypeGlow >* emitter = BuildEmitter<ParticleTypeGlow >(plist, group);
        EnergyParticleGroup<ParticleTypeGlow > *particleGroup = new EnergyParticleGroup<ParticleTypeGlow >(plist.GetInt(group + ".count"), emitter);
        
        // modifiers
        for (int i=0; i < plist.ListSize(group + ".modifiers"); i++ ) {
            string name = plist.GetString(group + ".modifiers", i);
            logger.info << "Adding modifier: " << name << logger.end;
            IModifier<ParticleTypeGlow >* mof = BuildModifier<ParticleTypeGlow >(plist, group + "." + name);
            if (mof != NULL)
                particleGroup->AddModifier(mof);
            
        }
    
        
        return make_pair(particleGroup,BuildRenderNode<ParticleTypeGlow,EnergyParticleGroup<ParticleTypeGlow> >(plist, group, particleGroup));
    } else {
        logger.info << "Unknown type: " << type << logger.end;
    }
    
    return make_pair<IParticleGroup*,IRenderNode*>(NULL,NULL);
}

template <class T> IEmitter<T>* ParticleGroupBuilder::BuildEmitter(PropertyList& plist, string group) {

    string type = plist.GetString(group + ".emitter.type");
    if (type == "Point") {
        PointEmitter<T >* emitter = new PointEmitter<T >(plist.GetInt(group + ".emitter.speed"));
        plist.SetIntP(&(emitter->speed), group + ".emitter.speed");
        emitter->SetPrototype(BuildParticle<T >(plist, group + ".emitter.prototype"));
        
        
        return emitter;
    } else {
        logger.info << "Unknown emitter type: " << type << logger.end;
    }
    return NULL;
}

template <class T> T* ParticleGroupBuilder::BuildParticle(PropertyList& plist, string group) {
    T* particle = new T();
    if (plist.HaveKey(group + ".texture"))
        particle->texr = ResourceManager<ITextureResource>::Create(plist.GetString(group + ".texture"));
    if (plist.HaveKey(group + ".energy")) {
        plist.SetFloatP(&(particle->energy), group + ".energy");
        plist.SetFloatP(&(particle->lifespan), group + ".energy");
    }
    if (plist.HaveKey(group + ".pos")) 
        plist.SetVectorP(&(particle->pos), group + ".pos");
    if (plist.HaveKey(group + ".size")) 
        plist.SetFloatP(&(particle->size), group + ".size");
    if (plist.HaveKey(group + ".color")) 
        plist.SetColorP(&(particle->color), group + ".color");

    
    
    return particle;
}


template <class T, class G> IRenderNode* ParticleGroupBuilder::BuildRenderNode(PropertyList& plist, string group, G* g) {
    string type = plist.GetString(group + ".render");
    if (type == "Billboard") {
        return new BillBoardRenderNode<T, G >(g);
    } else {
        logger.info << "Unkown render: " << type << logger.end;
        return NULL;
    }
}

template <class T> IModifier<T>* ParticleGroupBuilder::BuildModifier(PropertyList& plist, string group) {
    string type = plist.GetString(group + ".type");
    if (type == "PointerField") {
        
        string field = plist.GetString(group + ".field");
        if (field == "energy") {
            
            return new PointerFieldModifier<T , float> (&T::AddToEnergy, plist.GetFloatP(group + ".value"));
            
        } else {
            logger.info << "unknown field" << logger.end;
        }
            
        
    } else {
        logger.info << "Unknown modifier: " << type << logger.end;
    
        return NULL;
    }
}
