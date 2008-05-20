// Game Factory for an example project.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

// Class header
#include "GameFactory.h"

// OpenEngine library
#include <Display/Viewport.h>
#include <Display/ViewingVolume.h>
#include <Display/SDLFrame.h>
#include <Devices/AntSDLInput.h>
#include <Display/Frustum.h>
#include <Renderers/OpenGL/Renderer.h>
#include <Renderers/OpenGL/RenderingView.h>
#include <Scene/SceneNode.h>
#include <Utils/Statistics.h>
#include <Resources/TGAResource.h>
#include <Resources/OBJResource.h>
#include <Resources/DirectoryManager.h>
#include <Resources/ResourceManager.h>


// Example extension
#include <Particles/ParticleSystem.h>
#include <Particles/ParticleGroups.h>
#include <Particles/Particles.h>
#include <Particles/Modifiers.h>
#include <Math/Interpolators.h>
#include <Particles/PointEmitter.h>
#include <Utils/PropertyList.h>
#include <Display/AntTweakBarModule.h>

// Project files
#include "DownCameraEventHandler.h"
#include "CustomLevel.h"

#include "PlistBar.h"
#include "ParticleGroupBuilder.h"

// Additional namespaces (others are in the header).
using namespace OpenEngine::Devices;
using namespace OpenEngine::Renderers::OpenGL;
using namespace OpenEngine::Particles;
using namespace OpenEngine::Math;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Display;
using OpenEngine::EventHandlers::DownCameraEventHandler;
using OpenEngine::Level::CustomLevel;

/**
 * Game quit handler.
 *
 * This is an example of a keyboard listener.
 * Every time the a key is released a key (UP) event is sent to the
 * handle method. If the key symbol is escape it quits the game.
 *
 * Note that this will not work before you have implemented the
 * keyboard and mouse interfaces in SDLInput.
 *
 * The code to register the handler is found in the
 * GameFactory::SetupEngine method.
 */
class QuitEventHandler {
public:
    void HandleQuit(KeyboardEventArg arg) {
        if (arg.sym == KEY_ESCAPE) {
            //OpenEngine::Hello::World();
            IGameEngine::Instance().Stop();
        }
    }
};

class GLSettings : public IRenderNode {
public:
    void Apply(IRenderingView* view) {
        // Enable color material, including face colors when blending. 
        //glEnable(GL_COLOR_MATERIAL);
        
        GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
        GLfloat lm_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        
        glClearColor(.9f,
                     .9f,
                     .9f,
                     .0f);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);	// Turn Depth Testing Off
        
        VisitSubNodes(*view);
        
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }
};


#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

/**
 * Factory constructor.
 *
 * Initializes the different components so they are accessible when
 * the setup method is executed.
 */
GameFactory::GameFactory() {
    
    // Create a frame and viewport.
    this->frame = new SDLFrame(WINDOW_WIDTH, WINDOW_HEIGHT, 32);
    
    // Main viewport
    Viewport* viewport = new Viewport(*frame);
    camera = new Camera(*(new ViewingVolume()));
    camera->SetPosition(Vector<3,float>(-100,0,-10));
    camera->LookAt(Vector<3,float>(0,0,0));
    
    // Bind the camera to the viewport
    Frustum* frustum = new Frustum(*camera);
    frustum->SetFar(1000);
    viewport->SetViewingVolume(frustum);
    
    
    // Create a renderer.
    this->renderer = new Renderer();
    
    // Add a rendering view to the renderer
    this->renderer->AddRenderingView(new RenderingView(*viewport));
    
}

/**
 * Setup handler.
 *
 * This is the main setup method of the game factory. Here you can add
 * any non-standard modules and perform other setup tasks prior to the
 * engine start up.
 *
 * @param engine The game engine instance.
 */
bool GameFactory::SetupEngine(IGameEngine& engine) {
    
#define ParticleType BillBoardParticle<EnergyParticle<DirectionParticle<IParticle> > >
#define GroupType EnergyParticleGroup<ParticleType >
    
    string resourcedir = "./projects/PatSys/data/";
    DirectoryManager::AppendPath(resourcedir);
    
    ResourceManager<ITextureResource>::AddPlugin(new TGAPlugin());
    ResourceManager<IModelResource>::AddPlugin(new OBJPlugin());
    
    // Add your mouse and keyboard module here
    AntSDLInput* input = new AntSDLInput();
    ParticleSystem* system = new ParticleSystem();
//    PointEmitter<ParticleType> * emitter = new PointEmitter<ParticleType >(10); 
    
	PropertyList *plist = new PropertyList("particles.txt");
    
//	plist->SetIntP(&(emitter->speed), "emit.speed");
//    
//    
//    plist->SetFloatP(&(emitter->prototype->energy), "test.energy");
//    plist->SetFloatP(&(emitter->prototype->lifespan), "test.energy");
//    plist->SetVectorP(&(emitter->prototype->pos), "test.start");
//    plist->SetVectorP(&(emitter->prototype->direction), "test.dir");
//    //emitter->prototype->size = 5;
//    
//    // blending
//    plist->SetIntP(&(emitter->prototype->blendSrc), "proto.blendSrc");
//    plist->SetIntP(&(emitter->prototype->blendDst), "proto.blendDst");
//    
//    plist->SetFloatP(&(emitter->prototype->size), "test.size");
//    plist->SetColorP(&(emitter->prototype->color), "proto.color");
//    //emitter->prototype->color = Vector<4,float>(1,.5,.5,.5);
//    emitter->prototype->texr = ResourceManager<ITextureResource>::Create("particle.tga");
//    GroupType* group = new GroupType(1000, emitter);
//    
//	
//    
//	
//    //group->AddModifier(new StaticForceModifier<ParticleType >(Vector<3,float>(1,1,1)));
//	WobblyFieldModifier<ParticleType, Vector<3,float> > *wob = new WobblyFieldModifier<ParticleType, Vector<3,float> >
//	(&ParticleType::AddToPos, Vector<3,float>(.5,.7,.9));
//	
//	plist->SetBoolP(&(wob->active), "wobbly.active");
//	
//	//wob->active = plist->GetBool("wobbly.active");
//    group->AddModifier(wob);
    
    //     group->AddModifier(new WobblyFieldModifier<ParticleType, Vector<4,float> >
    //                        (&ParticleType::AddToColor, Vector<4,float>(.3,.3,.3,1)));
    
    
    //    group->AddModifier(new StaticFieldModifier<ParticleType, float >
    //                       (&ParticleType::AddToRotation, 10.0));
//	float *p = plist->GetFloatP("test.test");
//    
//    Vector<3,float> *pv = plist->GetVectorP<3,float>("test.vec");
//    
    
//    group->AddModifier(new PointerFieldModifier<ParticleType, float >
//                       (&ParticleType::AddToRotation, p));
//    
//    group->AddModifier(new PointerFieldModifier<ParticleType, Vector<3,float> >
//                       (&ParticleType::AddToPos, pv));
//    
//    //interpolator
//    LinearInterpolator<float> *sizeInterp = new LinearInterpolator<float>(3);
//    sizeInterp->AddPoint(0, 0, 10);
//    sizeInterp->AddPoint(1, .5, 30);
//    sizeInterp->AddPoint(2, 1, 0);
//    
//    InterpolatingFieldModifier<ParticleType, float, LinearInterpolator<float> >* sizeInMof = 
//    new InterpolatingFieldModifier<ParticleType, float, LinearInterpolator<float> >(&ParticleType::SetSize, sizeInterp );
//    group->AddModifier(sizeInMof);
//    
//
////    QuadraticInterpolator<float, 3> *sizeInterp = new QuadraticInterpolator<float, 3>();
////    sizeInterp->points[0] = 10.0;
////    sizeInterp->points[1] = 30.0;
////    sizeInterp->points[2] = 10.0;
////    InterpolatingFieldModifier<ParticleType, float, QuadraticInterpolator<float,3> >* sizeInMof = 
////    new InterpolatingFieldModifier<ParticleType, float, QuadraticInterpolator<float,3> >(&ParticleType::SetSize, sizeInterp );
////    group->AddModifier(sizeInMof);
////    
////    
//    group->AddModifier(new StaticFieldModifier<ParticleType, Vector<3,float> >
//                       (&ParticleType::AddToPos, Vector<3,float>(.3,.2,0)));
//    //group->AddModifier(new StaticEnergyModifier<ParticleType >(-10.0f));
//    group->AddModifier(new StaticFieldModifier<ParticleType, float>
//                       (&ParticleType::AddToEnergy, -1.0f));
//    
//    group->AddModifier(new FieldFieldModifier<ParticleType, Vector<3,float> >
//                       (&ParticleType::AddToPos, &ParticleType::GetDirection));
//    
//    //system->AddGroup(group);
//    


    ParticleGroupBuilder *groupBuilder = new ParticleGroupBuilder(*plist, string("p1"));
    
    system->AddGroup(groupBuilder->GetParticleGroup());

	AntTweakBarModule *tw = new AntTweakBarModule(WINDOW_WIDTH, WINDOW_HEIGHT);
    PlistBar *bar = new PlistBar(*plist);

    EnergyParticleGroup<BillBoardParticle<EnergyParticle<DirectionParticle<IParticle> > > > *group = (EnergyParticleGroup<BillBoardParticle<EnergyParticle<DirectionParticle<IParticle> > > >*)(groupBuilder->GetParticleGroup());

    Callback<EnergyParticleGroup<BillBoardParticle<EnergyParticle<DirectionParticle<IParticle> > > > >* goCB
    = 
        new
        Callback<EnergyParticleGroup<BillBoardParticle<EnergyParticle<DirectionParticle<IParticle>
        > >
        > >(*group,
            (&EnergyParticleGroup<BillBoardParticle<EnergyParticle<DirectionParticle<IParticle> > > >::Spawn));

    tw->AddBar(bar);
    goCB->name = "Go";
    bar->AddCallBack(goCB);


    
    
    DownCameraEventHandler* cameraHandler = new DownCameraEventHandler(camera,
                                                                       this);
    
	input->SetValidator(&AntTweakBarModule::HandleEvent);
	
    engine.AddModule(*input);
    engine.AddModule(*system, IGameEngine::TICK_DEPENDENT);
    engine.AddModule(*cameraHandler);
	engine.AddModule(*tw);
    
    
    
    
    // Create a root scene node
    SceneNode* scene = new SceneNode();
    
    
    PropertyList* pSimple = new PropertyList("levels/simple.lvl");
    CustomLevel* level = new CustomLevel(*pSimple);
    level->Load();
    //SceneNode* physNode = level->GetPhysicsGraph();
    SceneNode* staticScene = level->GetSceneGraph();
    
    scene->AddNode(staticScene);
    
	
    //SceneNode* glSet = new GLSettings();
//    scene->/*AddNode(glSet);
//     glSet->*/
//    AddNode(new BillBoardRenderNode<ParticleType ,GroupType >(group));
    
    scene->AddNode(groupBuilder->GetRenderNode());

    scene->AddNode(tw->RenderNode());
    
    //scene->AddNode(new BillBoardRenderNode<ParticleType ,GroupType >((GroupType*)groupBuilder->GetParticleGroup()));
    
    
    // Supply the scene to the renderer
    this->renderer->SetSceneRoot(scene);
    
    // Bind the quit handler (the keyboard module needs to work for
    // the handler to actually quit).
    // For more information on the event system please read:
    // http://openengine.dk/wiki/EventSystemGuide
    
    
    //Bind the camera handlers
    Listener<DownCameraEventHandler, MouseMovedEventArg>* cam_l
    = new Listener<DownCameraEventHandler, MouseMovedEventArg> (*cameraHandler, &DownCameraEventHandler::HandleMouseMoved);
    IMouse::mouseMovedEvent.Add(cam_l);
    
    Listener<DownCameraEventHandler, MouseButtonEventArg>* mdown
    = new Listener<DownCameraEventHandler, MouseButtonEventArg> (*cameraHandler, &DownCameraEventHandler::HandleMouseDown);
    IMouse::mouseDownEvent.Add(mdown);
    
    Listener<DownCameraEventHandler, MouseButtonEventArg>* mup
    = new Listener<DownCameraEventHandler, MouseButtonEventArg> (*cameraHandler, &DownCameraEventHandler::HandleMouseUp);
    IMouse::mouseUpEvent.Add(mup);
    
    
    QuitEventHandler* quit_h = new QuitEventHandler();
    Listener<QuitEventHandler, KeyboardEventArg>* quit_l
    = new Listener<QuitEventHandler, KeyboardEventArg> (*quit_h, &QuitEventHandler::HandleQuit);
    IKeyboard::keyUpEvent.Add(quit_l);
    
    // Add some module
    engine.AddModule(*(new OpenEngine::Utils::Statistics(1000)));
    
    // Return true to signal success.
    return true;
    
}

// Other factory methods. The returned objects are all created in the
// factory constructor.
IFrame*      GameFactory::GetFrame()    { return this->frame;    }
IRenderer*   GameFactory::GetRenderer() { return this->renderer; }
