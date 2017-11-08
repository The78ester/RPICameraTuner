#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    //ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(false);

	m_console_listener.setup(this);
	m_omx_camera_settings.width = 1280;
	m_omx_camera_settings.height = 720;
	m_omx_camera_settings.framerate = 30;
	m_omx_camera_settings.enableTexture = true;
	m_omx_camera_settings.enablePixels = false;
	m_omx_camera_settings.doRecording = false;

	m_video_grabber.setup(m_omx_camera_settings);
	m_video_grabber.setBurstMode(true);
	m_filter_collection.setup();

	m_background_color.setName("Background Color");
	m_background_color.set(ofColor(114, 144, 154));

	typedef std::shared_ptr < ofParameter<int> >			sp_param_int;
	typedef std::shared_ptr < ofParameter<bool> >			sp_param_bool;
	typedef std::shared_ptr < ofParameter<float> >			sp_param_float;
	typedef std::shared_ptr < ofParameter< ofxImGuiEnum > >	sp_param_enum;

	m_sp_param_shuter_speed = sp_param_gui_int (new ofParameter< ofxImGuiInt >("ShutterSpeed_MicroSec", ofxImGuiInt(m_video_grabber.getShutterSpeed(), 10, 100)));

	sp_param_int sp_sharpness	= sp_param_int(new ofParameter<int>("Sharpness", m_video_grabber.getSharpness(), -100, 100));
	sp_param_int sp_contrast	= sp_param_int(new ofParameter<int>("Contrast", m_video_grabber.getContrast(), -100, 100));
	sp_param_int sp_brightness	= sp_param_int(new ofParameter<int>("Brightness", m_video_grabber.getBrightness(), 0, 100));
	sp_param_int sp_saturation	= sp_param_int(new ofParameter<int>("Saturation", m_video_grabber.getSaturation(), -100, 100));
	sp_param_int sp_iso			= sp_param_int(new ofParameter<int>("ISO", m_video_grabber.getISO(), 100, 2000));
	sp_param_int sp_wb;
	sp_param_int sp_if;
	sp_param_int sp_es;
	sp_param_int sp_mm;
	sp_param_int sp_mr;

	sp_param_bool sp_is_auto_iso = sp_param_bool(new ofParameter<bool>("IsAutoISO", m_video_grabber.getAutoISO()));
	sp_param_int sp_dre = sp_param_int(new ofParameter<int>("DynRangeExp", m_video_grabber.getDRE(), 0, 3));
	sp_param_int sp_ev = sp_param_int(new ofParameter<int>("Ev Compensation", m_video_grabber.getEvCompensation(), -4, 4));

	m_param_group.setName("RPI Camera Settings");
	m_param_group.add(*sp_sharpness);
	m_param_group.add(*sp_contrast);
	m_param_group.add(*sp_brightness);
	m_param_group.add(*sp_saturation);
	m_param_group.add(*sp_is_auto_iso);
	m_param_group.add(*sp_iso);
	m_param_group.add(*sp_param_bool(new ofParameter<bool>("IsAutoShutter", m_video_grabber.getAutoShutter())));
	m_param_group.add(*m_sp_param_shuter_speed);
	m_param_group.add(*sp_dre);
	m_param_group.add(*sp_ev);


	{
		ofxImGuiEnum enum_val;
		enum_val.content.push_back("Off");
		enum_val.content.push_back("Auto");
		enum_val.content.push_back("SunLight");
		enum_val.content.push_back("Cloudy");
		enum_val.content.push_back("Shade");
		enum_val.content.push_back("Tungsten");
		enum_val.content.push_back("Fluorescent");
		enum_val.content.push_back("Incandescent");
		enum_val.content.push_back("Flash");
		enum_val.select = 0;

		m_param_group.add(*sp_param_enum(new ofParameter< ofxImGuiEnum >("WhiteBalance", enum_val)));

		sp_wb = sp_param_int(new ofParameter<int>("WhiteBalance", 0, 0, enum_val.content.size() - 1));
	}

	{
		ofxImGuiEnum enum_val;
		enum_val.content.push_back("None");
		enum_val.content.push_back("Noise");
		enum_val.content.push_back("Emboss");
		enum_val.content.push_back("Negative");
		enum_val.content.push_back("Sketch");
		enum_val.content.push_back("OilPaint");
		enum_val.content.push_back("Hatch");
		enum_val.content.push_back("Gpen");
		enum_val.content.push_back("Antialias");
		enum_val.content.push_back("DeRing");
		enum_val.content.push_back("Solarize");
		enum_val.select = 0;

		m_param_group.add(*sp_param_enum(new ofParameter< ofxImGuiEnum >("ImageFilter", enum_val)));

		sp_if = sp_param_int(new ofParameter<int>("ImageFilter", 0, 0, enum_val.content.size() - 1));
	}

	{
		ofxImGuiEnum enum_val;
		enum_val.content.push_back("Off");
		enum_val.content.push_back("Auto");
		enum_val.content.push_back("Night");
		enum_val.content.push_back("BackLight");
		enum_val.content.push_back("SpotLight");
		enum_val.content.push_back("Sports");
		enum_val.content.push_back("Snow");
		enum_val.content.push_back("Beach");
		enum_val.content.push_back("LargeAperture");
		enum_val.content.push_back("SmallApperture");
		enum_val.select = 0;

		m_param_group.add(*sp_param_enum(new ofParameter< ofxImGuiEnum >("Exposure", enum_val)));

		sp_es = sp_param_int(new ofParameter<int>("Exposure", 0, 0, enum_val.content.size() - 1));
	}

	{
		ofxImGuiEnum enum_val;
		enum_val.content.push_back("Average");
		enum_val.content.push_back("Spot");
		enum_val.content.push_back("Matrix");
		enum_val.select = 0;

		m_param_group.add(*sp_param_enum(new ofParameter< ofxImGuiEnum >("MeteringMode", enum_val)));

		sp_mm = sp_param_int(new ofParameter<int>("MeteringMode", 0, 0, enum_val.content.size() - 1));
	}

	{
		ofxImGuiEnum enum_val;
		enum_val.content.push_back("None");
		enum_val.content.push_back("Vertical");
		enum_val.content.push_back("Horizontal");
		enum_val.content.push_back("Both");
		enum_val.select = 0;

		m_param_group.add(*sp_param_enum(new ofParameter< ofxImGuiEnum >("Mirror", enum_val)));

		sp_mr = sp_param_int(new ofParameter<int>("Mirror", 0, 0, enum_val.content.size() - 1));
	}

	ofxImGuiParameter::Initialize();

	m_imgui_parameter.setup("RPICameraTuner", ofRectangle(10, 10, 480, 640));
	m_imgui_parameter.bind(m_background_color);
	m_imgui_parameter.bind(m_param_group);
	m_imgui_parameter.load();

	ofAddListener(m_param_group.parameterChangedE(), this, &ofApp::mf_on_parameter_changed);
	ofAddListener(ofxImGuiParameter::GetOnDrawEvent(), this, &ofApp::mf_on_ImGui_draw);

	m_param_group_midi.add(*sp_wb);
	m_param_group_midi.add(*sp_if);
	m_param_group_midi.add(*sp_es);
	m_param_group_midi.add(*sp_mm);
	m_param_group_midi.add(*sp_mr);

	ofAddListener(m_param_group_midi.parameterChangedE(), this, &ofApp::mf_on_parameter_midi_changed);

	m_midi_device.setup();
	m_midi_device.bind("Slider0", *sp_sharpness);
	m_midi_device.bind("Slider1", *sp_contrast);
	m_midi_device.bind("Slider2", *sp_brightness);
	m_midi_device.bind("Slider3", *sp_saturation);

	m_midi_device.bind("ButtonSolo4", *sp_is_auto_iso);
	m_midi_device.bind("Slider4", *sp_iso);

	m_midi_device.bind("Slider5", *sp_dre);
	m_midi_device.bind("Slider6", *sp_ev);

	m_midi_device.bind("Knob0", *sp_wb);
	m_midi_device.bind("Knob1", *sp_if);
	m_midi_device.bind("Knob2", *sp_es);
	m_midi_device.bind("Knob3", *sp_mm);
	m_midi_device.bind("Knob4", *sp_mr);

	m_can_not_modify_shutter.set(false);
	m_midi_device.bind("ButtonStop", m_can_not_modify_shutter);

	ofxMidiDevEvent* p_event = NULL;
	m_midi_device.get_event("Slider7", &p_event);
	ofAddListener(*p_event, this, &ofApp::mf_on_midi_changed);

	m_midi_device.get_event("Knob7", &p_event);
	ofAddListener(*p_event, this, &ofApp::mf_on_midi_changed);

	m_midi_device.get_event("ButtonMute7", &p_event);
	ofAddListener(*p_event, this, &ofApp::mf_on_midi_changed);

	m_midi_device.get_event("ButtonRecord7", &p_event);
	ofAddListener(*p_event, this, &ofApp::mf_on_midi_changed);
}

void ofApp::exit()
{
	ofxImGuiParameter::Finalize();
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (!m_video_grabber.isFrameNew())
	{
		return;
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{   
	ofSetBackgroundColor(m_background_color.get());

	m_video_grabber.draw();

	ofxImGuiParameter::Draw();
}

void ofApp::mf_on_parameter_changed(ofAbstractParameter& e)
{
	if (e.getName() == "Sharpness")
	{
		m_video_grabber.setSharpness(e.cast<int>().get());
	}
	else if (e.getName() == "Contrast")
	{
		m_video_grabber.setContrast(e.cast<int>().get());
	}
	else if (e.getName() == "Brightness")
	{
		m_video_grabber.setBrightness(e.cast<int>().get());
	}
	else if (e.getName() == "Saturation")
	{
		m_video_grabber.setSaturation(e.cast<int>().get());
	}
	else if (e.getName() == "IsAutoISO")
	{
		m_video_grabber.setAutoISO(e.cast<bool>().get());
	}
	else if (e.getName() == "ISO")
	{
		m_video_grabber.setISO(e.cast<int>().get());
	}
	else if (e.getName() == "IsAutoShutter")
	{
		m_video_grabber.setAutoShutter(e.cast<bool>().get());
	}
	else if (e.getName() == "ShutterSpeed_MicroSec")
	{
		m_video_grabber.setShutterSpeed(e.cast<int>().get());
	}
	else if (e.getName() == "DynRangeExp")
	{
		m_video_grabber.setDRE(e.cast<int>().get());
	}
	else if (e.getName() == "Ev Compensation")
	{
		m_video_grabber.setEvCompensation(e.cast<int>().get());
	}
	else if (e.getName() == "WhiteBalance")
	{
		ofParameter< ofxImGuiEnum >& param = e.cast<ofxImGuiEnum>();
		ofxImGuiEnum const& val = param.get();
		m_video_grabber.setWhiteBalance((OMX_WHITEBALCONTROLTYPE) val.select);
	}
	else if (e.getName() == "ImageFilter")
	{
		ofParameter< ofxImGuiEnum >& param = e.cast<ofxImGuiEnum>();
		ofxImGuiEnum const& val = param.get();
		m_video_grabber.setImageFilter((OMX_IMAGEFILTERTYPE) val.select);
	}
	else if (e.getName() == "Exposure")
	{
		ofParameter< ofxImGuiEnum >& param = e.cast<ofxImGuiEnum>();
		ofxImGuiEnum const& val = param.get();
		m_video_grabber.setExposurePreset((OMX_EXPOSURECONTROLTYPE) val.select);
	}
	else if (e.getName() == "MeteringMode")
	{
		ofParameter< ofxImGuiEnum >& param = e.cast<ofxImGuiEnum>();
		ofxImGuiEnum const& val = param.get();
		m_video_grabber.setMeteringType((OMX_METERINGTYPE) val.select);
	}
	else if (e.getName() == "Mirror")
	{
		ofParameter< ofxImGuiEnum >& param = e.cast<ofxImGuiEnum>();
		ofxImGuiEnum const& val = param.get();
		m_video_grabber.setMirror(val.select);
	}
}

void ofApp::mf_on_parameter_midi_changed(ofAbstractParameter& e)
{
	ofParameter< ofxImGuiEnum >& param =  m_param_group.get< ofxImGuiEnum >(e.getName());
	ofxImGuiEnum val = param.get();
	val.select = e.cast<int>().get();
	param.set(val);
}

void ofApp::mf_on_midi_changed(ofxMidiDevMsg& e)
{
	if (e.control_name == "ButtonMute7")
	{
		m_is_mute = e.message.value != 0;
	}
	else if (e.control_name == "ButtonRecord7")
	{
		m_is_record = e.message.value != 0;
	}
	else if (!m_can_not_modify_shutter.get())
	{
		int pos = 1;
		ofxImGuiInt val = m_sp_param_shuter_speed->get();

		if (m_is_record)
		{
			if (m_is_mute)
			{
				pos = 1000000;
			}
			else
			{
				pos = 100;
			}
		}
		else
		{
			if (m_is_mute)
			{
				pos = 10000;
			}
		}

		if (e.control_name == "Slider7")
		{

		}
		else if (e.control_name == "Knob7")
		{
			pos *= 10;
		}

		int my_val = static_cast<int>((e.message.value / 127.f) * 9);

		int tmp = val.value % pos;
		int tmp2 = val.value / pos;
		int tmp3 = tmp2 / 10;
		int tmp4 = tmp3 * 10 + my_val;
		int tmp5 = tmp4;
		if (pos > 1)
		{
			tmp5 = tmp4 * pos + tmp;
		}

		val.value = tmp5;
		m_sp_param_shuter_speed->set(val);
	}
}

void ofApp::mf_on_ImGui_draw()
{
	//You can test any ImGui Code here.
}

void ofApp::onCharacterReceived(KeyListenerEventData& event)
{
	//ofLog(OF_LOG_VERBOSE, "%c onCharacterReceived", event.character);
	keyPressed(event.character);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}


void ofApp::mouseScrolled(float x, float y){

}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
