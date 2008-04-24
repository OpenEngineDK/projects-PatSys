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

        if (type == "float") {
            
            TwAddVarRW(bar->GetBar(),
                       key.c_str(),
                       TW_TYPE_FLOAT,
                       p,
                       NULL);
        } else if (type == "vector3float") {
            Vector<3,float> *pv;
            pv = (Vector<3,float>*)p;
            TwAddVarRW(bar->GetBar(),
                       (key + "-x").c_str(),
                       TW_TYPE_FLOAT,
                       &(pv->GetElmPointer()[0]),
                       (string("label='x' group=") + key).c_str());
            TwAddVarRW(bar->GetBar(),
                       (key + "-y").c_str(),
                       TW_TYPE_FLOAT,
                       &(pv->GetElmPointer()[1]),
                       (string("label='y' group=") + key).c_str());
            TwAddVarRW(bar->GetBar(),
                       (key + "-z").c_str(),
                       TW_TYPE_FLOAT,
                       &(pv->GetElmPointer()[2]),
                       (string("label='z' group=") + key).c_str());
            
            logger.info << "Add "  << key.c_str() << logger.end;
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
