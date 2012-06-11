#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"
#include "cinder/Text.h"
#include "cinder/MayaCamUI.h"
#include "cinder/qtime/MovieWriter.h"
#include "cinder/Utilities.h"
#include <sstream>

#include "constants.h";

#include "ShapeContainer.h";


using namespace ci;
using namespace ci::app;
using namespace std;

using std::cout;




class reflection_animationApp : public AppBasic {
  public:
    
    
	void prepareSettings( Settings *settings );
	
	void setup();
	void update();
	void draw();
    
    void initShape();
	
	void resize( ResizeEvent event );
	void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void keyUp( KeyEvent event );
	
  public:
	// gui
	params::InterfaceGl m_params;
    // video
    qtime::MovieWriter  m_movieWriter;
    fs::path            m_path;
	
    // DISPLAY
    bool				m_showInterface;
    bool                m_recording;
    
	bool				m_showColour;
    RenderMode          m_renderMode;
    
    float               m_contrast;
    float               m_alpha;
    
    DisplayMode         m_face1Display;
    DisplayMode         m_face2Display;
    DisplayMode         m_face3Display;
    DisplayMode         m_face4Display;
    
    ColorA              m_dichroic1Colour;
    ColorA              m_dichroic2Colour;
	
	// CAMERA
	Vec3f				m_eye, m_center, m_up;
    MayaCamUI           m_mayaCam;

    Quatf				m_sceneRotation;
	float				m_cameraDistance;

	
	// STRUCTURE
	ShapeContainer      m_structure;
    
    ShapeType           m_shapeType;
    int                 m_numlevels;
    int                 m_incr;
    Vec3f               m_shapeDims;
    
    // ANIMATION
    AnimationMode       m_aniMode;
    
    // radar
    int                 m_interval;
    float               m_frontarea;
    float               m_backarea;
	
  private:
	vector<Capture>		m_capture;
    
    Surface8u           m_surface;
	vector<gl::Texture>	m_textures;
	vector<gl::Texture>	m_nameTextures;
	vector<Surface>		m_retainedSurfaces;
};

void reflection_animationApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1680, 1050 );
	settings->setFrameRate( 60.0f );
	settings->setFullScreen( false );
}

void reflection_animationApp::setup()
{
    
	// STRUCTURE
	
	m_shapeDims = Vec3f(50, 75, 50);
	m_numlevels = 12;
    m_incr = 2;
	
    m_shapeType = PYRAMID;
    
    initShape();
	cout << m_structure.m_shapes.size() << endl;
    
    // DISPLAY
    
    m_showInterface = true;
    m_recording = false;
    
    m_path = getHomeDirectory() / "Desktop/"; 
    
	m_showColour = true;
    m_renderMode = ALPHA;
    
    m_contrast = 4.0f;
    m_alpha = .45f;
    
    m_face1Display = ACTIVE;
    m_face2Display = DICHROIC1;
    m_face3Display = GLASS;
    m_face4Display = NONE;
    
    m_dichroic1Colour = Color(0.3f, 0.8f, 1.0f);
    m_dichroic2Colour = Color(1.0f, 0.8f, 0.3f);
	
    // ANIMATION
    
    m_aniMode = VIDEO;
    
    // radar
    m_interval = 360;
    m_frontarea = 1.5f;
    m_backarea  = 6.0f;
    
	// GUI
    
	m_params = params::InterfaceGl( "Geometry/Animation Parameters [press TAB to hide]", Vec2i( 400, 600 ) );
	
    // render
    m_params.addText( "render", "label=`Render`" );
	m_params.addSeparator();
    vector<string> rstrs; rstrs.push_back("ALPHA"); rstrs.push_back("ADDITIVE"); rstrs.push_back("MULTIPLY");
    m_params.addParam( "Render Mode", rstrs, (int*)&m_renderMode );
    m_params.addParam( "Use Colour", &m_showColour );
    m_params.addParam( "Alpha", &m_alpha, "min=0.001 max=2.0 step=0.1" );
	m_params.addParam( "Contrast", &m_contrast, "min=0.001 max=5.0 step=0.1" );
	m_params.addParam( "Dichroic Colour 1", &m_dichroic1Colour );
	m_params.addParam( "Dichroic Colour 2", &m_dichroic2Colour );
    m_params.addText( "emptyline1", "label=` `" );
    
    // structure
	m_params.addText( "structure", "label=`Structure`" );
	m_params.addSeparator();
    vector<string> dstrs; dstrs.push_back("ACTIVE"); dstrs.push_back("DICHROIC1"); dstrs.push_back("DICHROIC2"); dstrs.push_back("GLASS"); dstrs.push_back("NONE");
	m_params.addParam( "Face 1 Display mode", dstrs, (int*)&m_face1Display );
    m_params.addParam( "Face 2 Display mode", dstrs, (int*)&m_face2Display );
    m_params.addParam( "Face 3 Display mode", dstrs, (int*)&m_face3Display );
    m_params.addParam( "Face 4 Display mode", dstrs, (int*)&m_face4Display );
    m_params.addText( "emptyline2", "label=` `" );
    
    // generate shape
    m_params.addText( "generate", "label=`Generate`" );
	m_params.addSeparator();
    vector<string> sstrs; sstrs.push_back("PYRAMID"); sstrs.push_back("TETRAHEDRON"); sstrs.push_back("HOLLOW PYRAMID");
	m_params.addParam( "Shape", sstrs, (int*)&m_shapeType );
    m_params.addParam( "Number Of Levels", &m_numlevels );
    m_params.addParam( "Vertical Interval", &m_incr );
    m_params.addParam( "Shape Dimensions", &m_shapeDims );
    m_params.addButton( "Recreate Shape", std::bind( &reflection_animationApp::initShape, this ) );
    m_params.addText( "emptyline3", "label=` `" );
    
    // animation
    m_params.addText( "animation", "label=`Animation`" );
	m_params.addSeparator();
    vector<string> astrs; astrs.push_back("STATIC"); astrs.push_back("RADAR"); astrs.push_back("VIDEO"); astrs.push_back("VIDEO_RADAR");
	m_params.addParam( "Animation Mode", astrs, (int*)&m_aniMode );
    m_params.addParam( "Interval", &m_interval );
    m_params.addParam( "Size (front)", &m_frontarea );
    m_params.addParam( "Size (tail)", &m_backarea );
    
    m_params.addButton( "Recreate Shape", std::bind( &reflection_animationApp::initShape, this ) );
    
	
	// CAMERA/PERSPECTIVE (not updated anymore -> mayacam)

	m_cameraDistance = 800.0f;
    
    m_eye        = Vec3f( 0.0f, 0.0f, m_cameraDistance );
	m_center     = Vec3f::zero();
	m_up         = Vec3f::yAxis();
    
    CameraPersp cam;
    
    cam.setEyePoint(m_eye);
    cam.setCenterOfInterestPoint(m_center);
    cam.setWorldUp(m_up);
    cam.setPerspective(60.0, getWindowAspectRatio(), 5.0, 5000.0);
    m_mayaCam.setCurrentCam( cam );
    
	
	// CAPTURE/VIDEO/INPUT
    
    m_surface = Surface8u(600, 400, true);
    
	vector<Capture::DeviceRef> devices( Capture::getDevices() );
	for( vector<Capture::DeviceRef>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
		Capture::DeviceRef device = *deviceIt;
		console() << "Found Device " << device->getName() << " ID: " << device->getUniqueId() << std::endl;
		try {
			if( device->checkAvailable() ) {

				m_capture.push_back( Capture( 600, 400, device ) );
				m_capture.back().start();
				
				// placeholder texture
				m_textures.push_back( gl::Texture() );
			}
			else
				console() << "device is NOT available" << std::endl;
		}
		catch( CaptureExc & ) {
			console() << "Unable to initialize device: " << device->getName() << endl;
		}
	}
	
    // these should actually only be updated on change --> pointers to parameters
    
    DisplayMode* faceModesArr[] = {
        &m_face1Display, &m_face2Display, &m_face3Display, &m_face4Display
    };
    vector<DisplayMode*> p_faceModes(faceModesArr, faceModesArr+4);
    
    m_structure.setMaterialSettings( p_faceModes, &m_showColour, &m_alpha, &m_contrast );
    m_structure.setAnimationSettings( &m_aniMode );
    m_structure.setRadarSettings( &m_interval, &m_frontarea, &m_backarea);
    
    
}



void reflection_animationApp::update()
{
    
	for( vector<Capture>::iterator cIt = m_capture.begin(); cIt != m_capture.end(); ++cIt ) {
		if( cIt->checkNewFrame() ) {
			m_surface = cIt->getSurface();
			//m_textures[cIt - m_capture.begin()] = gl::Texture( m_surface );
			
		}
	}
    
    
    // update master shape
    
    int tNow = getElapsedFrames();
    
    m_structure.update(&m_surface, tNow);
	
}
	

void reflection_animationApp::draw()
{
    
	// BACKGROUND
    
	gl::clear( Color( 0.0025f, 0.0025f, 0.0025f ) ); 
	
	gl::enableDepthWrite( false );
	gl::enableDepthRead( false );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
    // RENDER MODE
    
    switch (m_renderMode) {
        case ALPHA:
            gl::enableAlphaBlending();
            break;
        case ADDITIVE:
            gl::enableAdditiveBlending();
            break;
        default:
            gl::enableAlphaBlending();
            break;
    }
    
    // DRAW

	// camera
	//m_eye        = Vec3f( 0.0f, 0.0f, m_cameraDistance );
	//m_camPers.lookAt( m_eye, m_center, m_up );
    
	// matrices
	gl::setMatrices( m_mayaCam.getCamera() );

	// draw pyramid
	m_structure.draw();
	
	// GUI
    
    if (m_recording) {
        Surface frame = copyWindowSurface();
        m_movieWriter.addFrame( frame );
    }
    
    
	if(m_showInterface) params::InterfaceGl::draw();
}

void reflection_animationApp::initShape()
{
    // initialise shapes
	m_structure.init(m_shapeType, m_numlevels, m_incr, m_shapeDims.x, m_shapeDims.y);
	
    
}

void reflection_animationApp::resize( ResizeEvent event )
{
    // reset camera on resize
	CameraPersp cam = m_mayaCam.getCamera();
    cam.setAspectRatio( getWindowAspectRatio() );
    m_mayaCam.setCurrentCam( cam );
}

void reflection_animationApp::mouseDown( MouseEvent event )
{    
    m_mayaCam.mouseDown( event.getPos() );
}

void reflection_animationApp::mouseDrag( MouseEvent event )
{
    m_mayaCam.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
}

void reflection_animationApp::keyUp( KeyEvent event )
{
    if (event.getCode() == KeyEvent::KEY_TAB) {
        m_showInterface = !m_showInterface;
    }
    
    if (event.getCode() == KeyEvent::KEY_SPACE) {
        Surface frame = copyWindowSurface();
        stringstream filename;
        filename << "geometry_screenshot_" << (getElapsedFrames()) << ".png";
        writeImage( m_path / filename.str(), frame);
    }
    
    if (event.getCode() == KeyEvent::KEY_r) {
        if (m_recording) {
            m_movieWriter.finish();
        }
        if (!m_recording) {
            stringstream filename;
            filename << "geometry_animation_" << (getElapsedFrames()) << ".mov";
            m_movieWriter = qtime::MovieWriter( m_path / filename.str(), getWindowWidth(), getWindowHeight() );
        }
        m_recording = !m_recording;
    }
}

CINDER_APP_BASIC( reflection_animationApp, RendererGl )
