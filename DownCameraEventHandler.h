#ifndef _DOWN_CAMERA_EVENT_HANDLER_H_
#define _DOWN_CAMERA_EVENT_HANDLER_H_

#include <Core/IGameFactory.h>
#include <Display/Camera.h>
#include <Devices/IMouse.h>


using namespace OpenEngine::Core;
using namespace OpenEngine::Display;
using namespace OpenEngine::Devices;

namespace OpenEngine {
    namespace EventHandlers {

        class DownCameraEventHandler : public IModule, 
                                       public IListener<MouseMovedEventArg>, 
                                       public IListener<MouseButtonEventArg> {

private:
    Camera* camera;
    int buttons;
    unsigned int oldX,oldY;
    IMouse* mouse;
    Vector<3,float> direction, up, position;
public:
    DownCameraEventHandler(Camera* camera,
                           IGameFactory* factory);
    ~DownCameraEventHandler();

    void Initialize();
    void Deinitialize();
    void Process(const float deltaTime, const float percent);
    bool IsTypeOf(const std::type_info& inf);
            
    void BindToEventSystem();
    
    void Handle(MouseMovedEventArg e);
    void Handle(MouseButtonEventArg e);
            



};

    }
}

#endif
