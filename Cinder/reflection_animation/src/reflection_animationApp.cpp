#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"
#include "cinder/Text.h"
#include "cinder/MayaCamUI.h"


#include "ShapeContainer.h"


using namespace uva;
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
	
    bool				m_showInterface;
	bool				m_showColour;
    bool                m_showHighContrast;
    bool                m_showDichroics;
	
	// camera
	Vec3f				m_eye, m_center, m_up;

    MayaCamUI           m_mayaCam;
	float				m_cameraDistance;
    
    Vec3f               m_pan;
	
	Quatf				m_sceneRotation;
	
	// shapes
	ShapeContainer      m_structure;
    
    ShapeType           m_shapeType;
    int                 m_numlevels;
    Vec3f               m_shapeDims;
	
  private:
	vector<Capture>		m_capture;
	vector<gl::Texture>	m_textures;
	vector<gl::Texture>	m_nameTextures;
	vector<Surface>		m_retainedSurfaces;
};

void reflection_animationApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1440, 900 );
	settings->setFrameRate( 60.0f );
	settings->setFullScreen( false );
}

void reflection_animationApp::setup()
{
	// shapes
	
	m_shapeDims = Vec3f(50, 60, 50);
	m_numlevels = 14;
	
    m_shapeType = PYRAMID;
    
	// initialise shapes
	m_structure.init(m_shapeType, m_numlevels, m_shapeDims.x, m_shapeDims.y);
	
	cout << m_structure.m_shapes.size() << endl;
	
	// gui
	m_params = params::InterfaceGl( "Reflections", Vec2i( 300, 600 ) );
	//m_params.addText( "emptyline1", "label=` `" );
	
	m_params.addText( "camera", "label=`Camera`" );
	m_params.addSeparator();	
	m_params.addParam( "Scene Rotation", &m_sceneRotation );
    m_params.addParam( "Panning", &m_pan, "min=0.0 max=2000.0 step=5.0 ");
    
	m_params.addParam( "Eye Distance", &m_cameraDistance, "min=50.0 max=2000.0 step=5.0 keyIncr== keyDecr=-" );
    
    
	m_params.addText( "emptyline2", "label=` `" );
	
	m_params.addText( "structure", "label=`Structure`" );
	m_params.addSeparator();
	m_params.addParam( "Use Colour", &m_showColour );
	m_params.addParam( "Show High Contrast", &m_showHighContrast );
	m_params.addParam( "Show Dichroics", &m_showDichroics );
    m_params.addText( "emptyline3", "label=` `" );
    
    m_params.addText( "generate", "label=`Generate`" );
	m_params.addSeparator();
	//m_params.addParam( "Shape", &m_shapeType );
    m_params.addParam( "Number of levels", &m_numlevels );
    m_params.addParam( "Shape Dimensions", &m_shapeDims );
    m_params.addButton( "Recreate Shape", std::bind( &reflection_animationApp::initShape, this ) );
	
	m_showInterface = true;
    m_showHighContrast = false;
	m_showColour = true;
    m_showDichroics = false;
	
	// virtual camera
	m_eye        = Vec3f( 0.0f, 0.0f, m_cameraDistance );
	m_center     = Vec3f::zero();
	m_up         = Vec3f::yAxis() * -1;
    
    m_pan        = Vec3f::zero();
	
	m_cameraDistance = 800.0f;
    
    CameraPersp cam;
    
    cam.setEyePoint(Vec3f(0, 0, m_cameraDistance));
    cam.setCenterOfInterestPoint(Vec3f::zero());
    cam.setPerspective(60.0, getWindowAspectRatio(), 5.0, 5000.0);
    m_mayaCam.setCurrentCam( cam );
    
	
	// capture
	// list out the devices
	vector<Capture::DeviceRef> devices( Capture::getDevices() );
	for( vector<Capture::DeviceRef>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
		Capture::DeviceRef device = *deviceIt;
		console() << "Found Device " << device->getName() << " ID: " << device->getUniqueId() << std::endl;
		try {
			if( device->checkAvailable() ) {
				m_capture.push_back( Capture( 300, 200, device ) );
				m_capture.back().start();
				
				// placeholder text
				m_textures.push_back( gl::Texture() );
				
				// render the name as a texture
				TextLayout layout;
				layout.setFont( Font( "Arial", 24 ) );
				layout.setColor( Color( 1, 1, 1 ) );
				layout.addLine( device->getName() );
				m_nameTextures.push_back( gl::Texture( layout.render( true ) ) );
			}
			else
				console() << "device is NOT available" << std::endl;
		}
		catch( CaptureExc & ) {
			console() << "Unable to initialize device: " << device->getName() << endl;
		}
	}
	
}



void reflection_animationApp::update()
{
	
	// update capture devices
	for( vector<Capture>::iterator cIt = m_capture.begin(); cIt != m_capture.end(); ++cIt ) {
		if( cIt->checkNewFrame() ) {
			Surface8u surf = cIt->getSurface();
			m_textures[cIt - m_capture.begin()] = gl::Texture( surf );
			
			// update pyramid
			m_structure.update(surf, m_showColour, m_showHighContrast);
			
		}
	}
	
}
	

void reflection_animationApp::draw()
{
	// clear out the window with black
	//gl::clear( Color( 0, 0, 0 ) ); 
	
	glClearColor( 0.0025f, 0.0025f, 0.0025f, 1 );
	gl::enableDepthWrite( false );
	gl::enableDepthRead( false );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
    //gl::enableAdditiveBlending();
    gl::enableAlphaBlending();

	
	
	// camera
	//m_eye        = Vec3f( 0.0f, 0.0f, m_cameraDistance );
	//m_camPers.lookAt( m_eye, m_center, m_up );
	
	// matrices
	gl::setMatrices( m_mayaCam.getCamera() );
    
	//gl::rotate( m_sceneRotation );
    //gl::translate( m_pan );
	
	// draw pyramid
	m_structure.draw(m_showDichroics);
	
	// gui
	if(m_showInterface) params::InterfaceGl::draw();
}

void reflection_animationApp::initShape()
{
    // initialise shapes
    //m_shapeType = (m_shapeType + 1)%2;
    m_shapeType = (m_shapeType == TETRA) ? m_shapeType = PYRAMID : m_shapeType = TETRA;
	m_structure.init(m_shapeType, m_numlevels, m_shapeDims.x, m_shapeDims.y);
	
}

void reflection_animationApp::resize( ResizeEvent event )
{
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
}

CINDER_APP_BASIC( reflection_animationApp, RendererGl )
