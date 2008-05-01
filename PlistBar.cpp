#include "PlistBar.h"
#include <Logging/Logger.h>

PlistBar::PlistBar(PropertyList& pl) : ITweakBar(pl.GetFileName()), plist(pl) {
    
}

PlistBar::~PlistBar() {}

void PlistBar::AddFields(ITweakBar* bar) {
    Callback<PlistBar>* rebindCB = 
    new Callback<PlistBar>(*this, &PlistBar::Rebind);
    
    TwAddButton(bar->GetBar(),
                "Rebind",
                ITweakBar::AntCallback<PlistBar>,
                rebindCB,
                NULL);
    
    
    map<string,pair<int,pair<string,void*> > > pointers = plist.GetFetctPointers();
    for (map<string,pair<int,pair<string,void*> > >::iterator 
         itr = pointers.begin();
         itr != pointers.end();
         itr++) {
        pair<string,pair<int,pair<string,void*> > > elm = *itr;
        string key = elm.first;
        //int idx = elm.second.first;
        string type = elm.second.second.first;
        void* p = elm.second.second.second;
        
		string group = PropertyList::GroupOf(key);
		string name = PropertyList::NameOf(key);
		
		logger.info << "Group for " << key << " is " << PropertyList::GroupOf(key) << logger.end;
		
        if (type == "float") {
            
            TwAddVarRW(bar->GetBar(),
                       key.c_str(),
                       TW_TYPE_FLOAT,
                       p,
                       (string("group=") + group + " label=" + name ).c_str() );
			
			string metaKey = "meta." + name + ".min";
			if (plist.HaveKey(metaKey)) {
				TwDefine((string(TwGetBarName(bar->GetBar())) + "/" + key + " min="+plist.GetString(metaKey)).c_str());
			}
			metaKey = "meta." + name + ".max";
			if (plist.HaveKey(metaKey)) {
				TwDefine((string(TwGetBarName(bar->GetBar())) + "/" + key + " max="+plist.GetString(metaKey)).c_str());
			}
			
			
		} else if (type == "bool") {
			
			TwAddVarRW(bar->GetBar(),
					   key.c_str(), 
					   TW_TYPE_BOOLCPP, 
					   p, 
					   (string("group=") + group + " label=" + name).c_str());
		} else if (type == "int") {
			
			TwAddVarRW(bar->GetBar(),
					   key.c_str(), 
					   TW_TYPE_INT32, 
					   p, 
					   (string("group=") + group + " label=" + name).c_str());
			
			
			
			
			
        } else if (type == "vector3float") {
            Vector<3,float> *pv;
            pv = (Vector<3,float>*)p;
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
