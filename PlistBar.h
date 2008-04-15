#ifndef _PLIST_BAR_H_
#define _PLIST_BAR_H_

#include <Display/ITweakBar.h>
#include <Utils/PropertyList.h>

using namespace OpenEngine::Display;
using namespace OpenEngine::Utils;

class PlistBar : public ITweakBar {

private:
    PropertyList& plist;
public:
    PlistBar(PropertyList& plist);
    ~PlistBar();
    
    void AddFields(ITweakBar* bar);
    void Save();
    void Reload();
    void Rebind();

};

#endif
