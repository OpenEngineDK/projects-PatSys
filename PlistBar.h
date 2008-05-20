#ifndef _PLIST_BAR_H_
#define _PLIST_BAR_H_

#include <Display/ITweakBar.h>
#include <Utils/PropertyList.h>

#include <list>

using namespace OpenEngine::Display;
using namespace OpenEngine::Utils;
using namespace std;

class PlistBar : public ITweakBar {

private:
    PropertyList& plist;
    list<ICallback*> callbacks;
public:
    PlistBar(PropertyList& plist);
    ~PlistBar();
    
    void AddFields(ITweakBar* bar);
    void AddCallBack(ICallback *cb);
    void Save();
    void Reload();
    void Rebind();

};

#endif
