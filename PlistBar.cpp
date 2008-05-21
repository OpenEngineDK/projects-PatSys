#include "PlistBar.h"
#include <Logging/Logger.h>

PlistBar::PlistBar(PropertyList& pl) : ITweakBar(pl.GetFileName()), plist(pl) {
    
}

PlistBar::~PlistBar() {}

void PlistBar::AddCallBack(ICallback *cb) {
    callbacks.push_back(cb);
}

void PlistBar::AddFields(ITweakBar* bar) {
    Callback<PlistBar>* rebindCB = 
    new Callback<PlistBar>(*this, &PlistBar::Rebind);
    
    rebindCB->name = "Rebind";
    AddCallBack(rebindCB);

    Callback<PropertyList>* saveCB = 
        new Callback<PropertyList>(plist,
                                   &PropertyList::Save);
    saveCB->name = "Save";
    AddCallBack(saveCB);


    for (list<ICallback*>::iterator itr = callbacks.begin();
         itr != callbacks.end();
         itr++) {
        TwAddButton(bar->GetBar(),
                    (*itr)->name.c_str(),
                    ITweakBar::AntCallback,
                    *itr,
                    NULL);
    }
    
    BindingsManager *mgr = plist.GetBindingsManager();
    
    set<string> keys = plist.GetBoundKeys();


    for (set<string>::iterator itr = keys.begin();
         itr != keys.end();
         itr++) {

        string key = *itr;
        
        string type = mgr->TypeFor(key);
        string group = PropertyList::GroupOf(key);
        string name = PropertyList::NameOf(key);
        if (typeid(float).name() == type) {
            TwAddVarRW(bar->GetBar(),
                       (key).c_str(),
                       TW_TYPE_FLOAT,
                       mgr->GetPointer<float>(key),
                       (string("group=") + group + " label=" + name ).c_str() );
            string metaKey = "meta." + name + ".min";
            if (plist.HaveKey(metaKey)) {
                TwDefine((string(TwGetBarName(bar->GetBar())) + "/" + key + " min="+plist.GetString(metaKey)).c_str());
            }
            metaKey = "meta." + name + ".max";
            if (plist.HaveKey(metaKey)) {
                TwDefine((string(TwGetBarName(bar->GetBar())) + "/" + key + " max="+plist.GetString(metaKey)).c_str());
            }
            
                        
        } else if (typeid(int).name() == type) {

            TwAddVarRW(bar->GetBar(),
					   key.c_str(), 
					   TW_TYPE_INT32, 
					   mgr->GetPointer<int>(key), 
					   (string("group=") + group + " label=" + name).c_str());
			
			
            string metaKey = "meta." + name + ".min";
			if (plist.HaveKey(metaKey)) {
				TwDefine((string(TwGetBarName(bar->GetBar())) + "/" + key + " min="+plist.GetString(metaKey)).c_str());
			}
			metaKey = "meta." + name + ".max";
			if (plist.HaveKey(metaKey)) {
				TwDefine((string(TwGetBarName(bar->GetBar())) + "/" + key + " max="+plist.GetString(metaKey)).c_str());
            }
        } else if (typeid(bool).name() == type) {
            TwAddVarRW(bar->GetBar(),
					   key.c_str(), 
					   TW_TYPE_BOOLCPP, 
					   mgr->GetPointer<bool>(key), 
					   (string("group=") + group + " label=" + name).c_str());
        } else if (typeid(Vector<3,float>).name() == type) {
            Vector<3,float> *pv;
            pv = mgr->GetPointer<Vector<3,float> >(key);
            TwAddVarRW(bar->GetBar(),
                       (key + "-x").c_str(),
                       TW_TYPE_FLOAT,
					   &((*pv)[0]),
                       (string("label='x' group=") + key).c_str());
            TwAddVarRW(bar->GetBar(),
                       (key + "-y").c_str(),
                       TW_TYPE_FLOAT,
					   &((*pv)[1]),
                       (string("label='y' group=") + key).c_str());
            TwAddVarRW(bar->GetBar(),
                       (key + "-z").c_str(),
                       TW_TYPE_FLOAT,
					   &((*pv)[2]),
                       (string("label='z' group=") + key).c_str());
			TwDefine((string(TwGetBarName(bar->GetBar())) + "/" + key + " group=" + group + " label=" + name).c_str());
        } else if (typeid(Vector<4,float>).name() == type &&
                   mgr->MetaFor(key) == "color") {
            Vector<4,float> *pv;
            pv = mgr->GetPointer<Vector<4,float> >(key);
			TwAddVarRW(bar->GetBar(),
					   key.c_str(), 
					   TW_TYPE_COLOR4F, 
					   &((*pv)[0]), 
					   (string("group=") + group + " label=" + name).c_str());
        } else {
            logger.info << "PlistBar: Unhandled type: " 
                        << type << " with meta: "
                        << mgr->MetaFor(key)
                        << logger.end;
        }
    }    
}

void PlistBar::Save() {
    // save plist
}

void PlistBar::Rebind() {
    TwRemoveAllVars(GetBar());
    AddFields(this);
}
