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
        FieldType type = TypeForField(field);
        logger.info << "type is: " << type << logger.end;
        switch (type) {
            case FT_FLOAT: {
                void (T::*ptr)(float);
                //ptr = T::AddMethodForField<float>(field).first;
                ptr = MethodForField_float<T>(field).first;
                logger.info << "adding Pointer modifier" << logger.end;
                PointerFieldModifier<T, float> *pfm = 
                new PointerFieldModifier<T, float> (ptr, plist.GetFloatP(group + ".value"));
                plist.SetBoolP(&(pfm->active), group + ".active");
                return pfm;
            }
            case FT_VECTOR3F: {
                void (T::*ptr)(Vector<3,float> );
                ptr = MethodForField_vec3f<T>(field).first;
                logger.info << "adding Pointer modifier" << logger.end;
                PointerFieldModifier<T, Vector<3,float> > *pfm = 
                new PointerFieldModifier<T, Vector<3,float> > (ptr, plist.GetVectorP<3,float>(group + ".value"));
                plist.SetBoolP(&(pfm->active), group + ".active");
                return pfm;
                
            }
            default:
                logger.info << "error, type not found" << logger.end;
        }
    } else if (type == "WobblyField") {
        string field = plist.GetString(group + ".field");
        FieldType type = TypeForField(field);
        logger.info << "type is: " << type << logger.end;
        switch (type) {
            case FT_FLOAT: {
                void (T::*ptr)(float);
                //ptr = T::AddMethodForField<float>(field).first;
                ptr = MethodForField_float<T>(field).first;
                logger.info << "adding Pointer modifier" << logger.end;
                WobblyFieldModifier<T, float> *pfm = 
                new WobblyFieldModifier<T, float> (ptr, plist.GetFloat(group + ".value"));
                plist.SetFloatP(&(pfm->value), group + ".value");
                plist.SetBoolP(&(pfm->active), group + ".active");
                return pfm;
            }
            case FT_VECTOR3F: {
                void (T::*ptr)(Vector<3,float> );
                ptr = MethodForField_vec3f<T>(field).first;
                logger.info << "adding Pointer modifier" << logger.end;
                WobblyFieldModifier<T, Vector<3,float> > *pfm = 
                new WobblyFieldModifier<T, Vector<3,float> > (ptr, plist.GetVector<3,float>(group + ".value"));
                plist.SetVectorP<3,float>(&(pfm->value), group + ".value");
                plist.SetBoolP(&(pfm->active), group + ".active");
                return pfm;
                
            }
            default:
                logger.info << "error, type not found" << logger.end;
        }
    } else {
        logger.info << "Unknown modifier: " << type << logger.end;
    
        
        return NULL;
    }
    return NULL;
}

template <class T> pair<void (T::*)(float), int> ParticleGroupBuilder::MethodForField_float(string field) {
    if (field == "energy") {
        return make_pair(&T::AddToEnergy,0);
    }
    logger.info << "field not found: " << field << logger.end;
    return make_pair<void (T::*)(float), int>(NULL, -1);
}

template <class T> pair<void (T::*)(Vector<3,float> ), int> ParticleGroupBuilder::MethodForField_vec3f(string field) {
    if (field == "pos") {
        return make_pair(&T::AddToPos,0);
    }
    logger.info << "field not found: " << field << logger.end;
    return make_pair<void (T::*)(Vector<3,float> ), int>(NULL, -1);
}


//
//template <class T> pair<void (T::*)(F), int> ParticleGroupBuilder::MethodForField< Vector<3,float> (string field) {
//    if (field == "energy") {
//        return make_pair(&T::AddToEnergy,0);
//    }
//    logger.info << "field not found: " << field << logger.end;
//    return make_pair<void (T::*)(F), int>(NULL, -1);
//}
//

FieldType ParticleGroupBuilder::TypeForField(string field) {
    if (field == "energy")
        return FT_FLOAT;
    else if (field == "pos")
        return FT_VECTOR3F;
    
    logger.info << "unknown field: " << field << logger.end;
    
    return FT_NONE;
    
}
