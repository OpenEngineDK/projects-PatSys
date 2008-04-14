#include "DownCameraEventHandler.h"

namespace OpenEngine {
    namespace EventHandlers {


DownCameraEventHandler::DownCameraEventHandler(Camera* camera,
                                               IGameFactory* factory) 
    : camera(camera),buttons(0) {
    direction = Vector<3,float>(1.0,0.0,0.0);
    up = Vector<3,float>(0.0,1.0,0.0);
    camera->SetDirection(direction, up);
    position = camera->GetPosition();
}

DownCameraEventHandler::~DownCameraEventHandler() {
}

void DownCameraEventHandler::Initialize() {
    mouse = dynamic_cast<IMouse*>(IGameEngine::Instance().Lookup(typeid(IMouse)));
    
}
void DownCameraEventHandler::Deinitialize() {}
void DownCameraEventHandler::Process(const float deltaTime, const float percent) {}
        bool DownCameraEventHandler::IsTypeOf(const std::type_info& inf) { return true; }

void DownCameraEventHandler::HandleMouseMoved(MouseMovedEventArg e) {
    
    if (buttons & BUTTON_LEFT) {
        if (e.x == oldX && e.y == oldY)
            return;
        // rotate
        float sensitivity = 0.005;

        direction += (direction % up) * (e.dx*sensitivity);
        direction.Normalize();
        camera->SetDirection(direction,up);
        
        //tilt
        Quaternion<float> rotation(e.dy * -sensitivity, direction % up);
        direction = rotation.RotateVector(direction);
        //up = rotation.RotateVector(up);
        camera->SetDirection(direction,up);
        

        mouse->SetCursor(oldX,oldY);
    } else if (buttons & BUTTON_RIGHT) {
        if (e.x == oldX && e.y == oldY)
            return;

        //move the camera
        //move forward
        position += -(direction * e.dy);
            
        //move sideways
        position += (direction % up) * e.dx;
        
        camera->SetPosition(position);

        mouse->SetCursor(oldX,oldY);
    }
      

}
void DownCameraEventHandler::HandleMouseDown(MouseButtonEventArg e) {
    buttons = e.state.buttons;
    oldX = e.state.x;
    oldY = e.state.y;
    mouse->HideCursor();
}
void DownCameraEventHandler::HandleMouseUp(MouseButtonEventArg e) {
    buttons = e.state.buttons;
    if (buttons == 0)
        mouse->ShowCursor();
}

    }
}
