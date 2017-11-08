#pragma once

#include "ofMain.h"
#include <ofxImGuiParameter.h>
#include <TerminalListener.h>
#include <ofxRPiCameraVideoGrabber.h>
#include <ImageFilterCollection.h>
#include <ofxMidiDevice.h>

class ofApp : public ofBaseApp, public KeyListener
{
public:
    void setup();
    void update();
    void draw();
	void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void mouseScrolled(float x, float y);
    
    void onCharacterReceived(KeyListenerEventData& event);

private:
    typedef std::shared_ptr < ofParameter< ofxImGuiInt > >	sp_param_gui_int;
    //Camera
    TerminalListener 			m_console_listener;
	ofxRPiCameraVideoGrabber 	m_video_grabber;
	OMXCameraSettings 			m_omx_camera_settings;
	ImageFilterCollection 		m_filter_collection;

	//Gui
	ofParameter< ofColor >		m_background_color;
	ofxImGuiParameter			m_imgui_parameter;
	ofParameterGroup			m_param_group;

	ofParameterGroup			m_param_group_midi;

	ofParameter< bool >			m_can_not_modify_shutter;
	sp_param_gui_int			m_sp_param_shuter_speed;

	ofxMidiDevice				m_midi_device;
	bool						m_is_mute;
	bool						m_is_record;

	void mf_on_parameter_changed(ofAbstractParameter& e);
	void mf_on_parameter_midi_changed(ofAbstractParameter& e);
	void mf_on_midi_changed(ofxMidiDevMsg& e);
	void mf_on_ImGui_draw();
};
