#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "WebSocketClient.h"
#include "cinder/Json.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class WebsocketPPTestApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	void update();
	void draw();
    
private:
    //These functions are built into the WebsocketPP block.
    void connect();
    void disconnect();
    WebSocketClient mClient;
    void onConnect();
	void onDisconnect();
	void onError( std::string err );
	void onRead( std::string msg );
	void write( std::string msg);
    
    Vec2f pos;
};

void WebsocketPPTestApp::connect()
{
	mClient.connect( "ws://localhost:8081" ); //Connect to port 8081
}

void WebsocketPPTestApp::disconnect()
{
	mClient.disconnect();
}

void WebsocketPPTestApp::onConnect()
{
    //When we connect, send a message to the server.
	console() << "connected" << endl;
    string data = "init-cinder";
    mClient.write( data );
}

void WebsocketPPTestApp::onDisconnect()
{
	console() << "disconnected" << endl;
}


void WebsocketPPTestApp::onError( string err )
{
}

void WebsocketPPTestApp::onRead( string msg )
{
    console() << msg << endl;
    
    //Similar to server.js: split the string by comma
    vector<string> parts = split(msg, ",");
    string event = parts[0];
    
    //If our event is 'change-pos', change the pos variable.
    if ( event == "change-pos" ) {
        pos.x = fromString<float>(parts[1]);
        pos.y = fromString<float>(parts[2]);
    }
}

void WebsocketPPTestApp::write( string msg )
{
    mClient.write( msg );
}

void WebsocketPPTestApp::setup()
{
    //Add callbacks so these events trigger.
    mClient.addConnectCallback( &WebsocketPPTestApp::onConnect, this );
	mClient.addDisconnectCallback( &WebsocketPPTestApp::onDisconnect, this );
	mClient.addErrorCallback( &WebsocketPPTestApp::onError, this );
	mClient.addReadCallback( &WebsocketPPTestApp::onRead, this );
    
    //We need to connect to the server when we open the app.
    connect();
}

void WebsocketPPTestApp::mouseDown( MouseEvent event )
{
}

void WebsocketPPTestApp::keyDown( KeyEvent event )
{
}

void WebsocketPPTestApp::update()
{
    mClient.poll(); //Not 100% sure what this does. You need it, though.
}

void WebsocketPPTestApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    //Draw that beautiful, beautiful circle.
    gl::drawSolidCircle( pos, 50 );
}

CINDER_APP_NATIVE( WebsocketPPTestApp, RendererGl )
