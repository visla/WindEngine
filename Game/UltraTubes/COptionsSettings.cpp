/*
    COptionsSettings.spp
	Autor: Tomislav Kukic
	Date: 17.3.2006.
	Version: 1.0    
*/



#include <COptionsSettings.h>
#include <CControllerSettings.h>
#include <WeFileStream.h>


#define MIN_KEYB_SENS	0.05f
#define MAX_KEYB_SENS	2.0f
#define MIN_MOUSE_SENS	0.1f
#define MAX_MOUSE_SENS	2.0f
#define MIN_JOY_SENS	0.1f
#define MAX_JOY_SENS	2.0f
#define MIN_JOY_DEAD	0.0f
#define MAX_JOY_DEAD	0.5f


COptionsSettings* COptionsSettings::Instance = new COptionsSettings();

COptionsSettings::COptionsSettings()
{
	SetDefaultSettings();
}


void COptionsSettings::Save()
{
	WeFileStream FS;
	FILE* f = fopen("options.cfg","rb");
	if(!f)
	{
		FS.OpenNew("options.cfg");
	}
	else
	{
		fclose(f);
		FS.Open("options.cfg");
	}

	FS.WriteInt(-6);
	FS.WriteInt(ResWidth);
	FS.WriteInt(ResHeight);
	FS.WriteInt(TextureQuality);
	FS.WriteInt(ShaderQuality);
	FS.WriteBool(PostProcess);
	FS.WriteBool(Glow);
	FS.WriteBool(Particles);
	FS.WriteFloat(MusicVolume);
	FS.WriteFloat(SoundVolume);
	FS.WriteFloat(KeyboardSensitivity);
	FS.WriteFloat(MouseSensitivity);
	FS.WriteFloat(JoystickSensitivity);
	FS.WriteFloat(JoystickDeadzone);
	FS.WriteInt(PhysicsQuality);

	FS.WriteBool(Shadows);
	FS.WriteBool(Projektors);
	FS.WriteBool(Flares);
	FS.WriteBool(MotionBlur);

	FS.WriteInt(Antialiasing);
	FS.WriteInt(Anisotropic);
	FS.WriteBool(Specular);
	FS.WriteBool(Bumping);
	FS.WriteBool(Refraction);

	FS.Close();
}


void COptionsSettings::Load()
{
	WeFileStream FS;

	if (!FS.Exists("options.cfg"))
	{
		return;
	}

	if (!FS.Open("options.cfg"))
	{
		return;
	}

	int ver = FS.ReadInt(); // negative version numbers
	
	if (ver > 0)
	{
		ResWidth = ver;
		ResHeight = FS.ReadInt();
		TextureQuality = FS.ReadInt();
		ShaderQuality = FS.ReadInt();
		PostProcess = FS.ReadBool();
		//AntiAliasing = FS.ReadBool();
		Glow = FS.ReadBool();
		Particles = FS.ReadBool();
		MusicVolume = (float)FS.ReadInt();
		SoundVolume = (float)FS.ReadInt();
		PhysicsQuality = FS.ReadInt();

		ControllerSettings.Deserialize(FS);
	}

	else if (ver == -1)
	{
		ResWidth = FS.ReadInt();
		ResHeight = FS.ReadInt();
		TextureQuality = FS.ReadInt();
		ShaderQuality = FS.ReadInt();
		PostProcess = FS.ReadBool();
		//AntiAliasing = FS.ReadBool();
		Glow = FS.ReadBool();
		Particles = FS.ReadBool();
		MusicVolume = (float)FS.ReadInt();
		SoundVolume = (float)FS.ReadInt();
		PhysicsQuality = FS.ReadInt();

		ControllerSettings.Deserialize(FS);

		// added in version -1
		Shadows = FS.ReadBool();
		Projektors = FS.ReadBool();
		Flares = FS.ReadBool();
		MotionBlur = FS.ReadBool();
	}	
	
	else if (ver == -2)
	{

		ResWidth = FS.ReadInt();
		ResHeight = FS.ReadInt();
		TextureQuality = FS.ReadInt();
		ShaderQuality = FS.ReadInt();
		PostProcess = FS.ReadBool();
		Glow = FS.ReadBool();
		Particles = FS.ReadBool();
		MusicVolume = (float)FS.ReadInt();
		SoundVolume = (float)FS.ReadInt();
		PhysicsQuality = FS.ReadInt();

		ControllerSettings.Deserialize(FS);

		Shadows = FS.ReadBool();
		Projektors = FS.ReadBool();
		Flares = FS.ReadBool();
		MotionBlur = FS.ReadBool();

		// added in version -2
		Antialiasing = FS.ReadInt();
		Anisotropic = FS.ReadInt();
		Specular = FS.ReadBool();
		Bumping = FS.ReadBool();
	}	

	else if (ver == -3)
	{

		ResWidth = FS.ReadInt();
		ResHeight = FS.ReadInt();
		TextureQuality = FS.ReadInt();
		ShaderQuality = FS.ReadInt();
		PostProcess = FS.ReadBool();
		Glow = FS.ReadBool();
		Particles = FS.ReadBool();
		MusicVolume = FS.ReadFloat();
		SoundVolume = FS.ReadFloat();
		PhysicsQuality = FS.ReadInt();

		ControllerSettings.Deserialize(FS);

		Shadows = FS.ReadBool();
		Projektors = FS.ReadBool();
		Flares = FS.ReadBool();
		MotionBlur = FS.ReadBool();

		Antialiasing = FS.ReadInt();
		Anisotropic = FS.ReadInt();
		Specular = FS.ReadBool();
		Bumping = FS.ReadBool();
	}	

	else if (ver == -4)
	{

		ResWidth = FS.ReadInt();
		ResHeight = FS.ReadInt();
		TextureQuality = FS.ReadInt();
		ShaderQuality = FS.ReadInt();
		PostProcess = FS.ReadBool();
		Glow = FS.ReadBool();
		Particles = FS.ReadBool();
		MusicVolume = FS.ReadFloat();
		SoundVolume = FS.ReadFloat();
		PhysicsQuality = FS.ReadInt();

		Shadows = FS.ReadBool();
		Projektors = FS.ReadBool();
		Flares = FS.ReadBool();
		MotionBlur = FS.ReadBool();

		Antialiasing = FS.ReadInt();
		Anisotropic = FS.ReadInt();
		Specular = FS.ReadBool();
		Bumping = FS.ReadBool();
	}	

	else if (ver == -5)
	{
		ResWidth = FS.ReadInt();
		ResHeight = FS.ReadInt();
		TextureQuality = FS.ReadInt();
		ShaderQuality = FS.ReadInt();
		PostProcess = FS.ReadBool();
		Glow = FS.ReadBool();
		Particles = FS.ReadBool();
		MusicVolume = FS.ReadFloat();
		SoundVolume = FS.ReadFloat();
		KeyboardSensitivity = FS.ReadFloat();
		MouseSensitivity = FS.ReadFloat();
		JoystickSensitivity = FS.ReadFloat();
		JoystickDeadzone = FS.ReadFloat();
		PhysicsQuality = FS.ReadInt();

		Shadows = FS.ReadBool();
		Projektors = FS.ReadBool();
		Flares = FS.ReadBool();
		MotionBlur = FS.ReadBool();

		Antialiasing = FS.ReadInt();
		Anisotropic = FS.ReadInt();
		Specular = FS.ReadBool();
		Bumping = FS.ReadBool();
	}	

	else if (ver == -6)
	{
		ResWidth = FS.ReadInt();
		ResHeight = FS.ReadInt();
		TextureQuality = FS.ReadInt();
		ShaderQuality = FS.ReadInt();
		PostProcess = FS.ReadBool();
		Glow = FS.ReadBool();
		Particles = FS.ReadBool();
		MusicVolume = FS.ReadFloat();
		SoundVolume = FS.ReadFloat();
		KeyboardSensitivity = FS.ReadFloat();
		MouseSensitivity = FS.ReadFloat();
		JoystickSensitivity = FS.ReadFloat();
		JoystickDeadzone = FS.ReadFloat();
		PhysicsQuality = FS.ReadInt();

		Shadows = FS.ReadBool();
		Projektors = FS.ReadBool();
		Flares = FS.ReadBool();
		MotionBlur = FS.ReadBool();

		Antialiasing = FS.ReadInt();
		Anisotropic = FS.ReadInt();
		Specular = FS.ReadBool();
		Bumping = FS.ReadBool();
		Refraction = FS.ReadBool();
	}	

	FS.Close();
}


void COptionsSettings::SetDefaultSettings()
{
	ResWidth = 800;
	ResHeight = 600;
	TextureQuality = 1;
	ShaderQuality = 1;
	PostProcess = false;
	Particles = true;
	Glow = false;
	MusicVolume = 1.0f;
	SoundVolume = 1.0f;
	PhysicsQuality = 1;
	Shadows = false;
	Projektors = false;
	Flares = false;
	MotionBlur = false;
	Antialiasing = 0;
	Anisotropic = 0;
	Specular = false;
	Bumping = false;
	Refraction = false;
	SetDefaultSensitivity();
}


float COptionsSettings::GetRelativeKeyboardSensitivity()
{
	return (KeyboardSensitivity - MIN_KEYB_SENS) / (MAX_KEYB_SENS - MIN_KEYB_SENS);
}

float COptionsSettings::GetRelativeMouseSensitivity()
{
	return (MouseSensitivity - MIN_MOUSE_SENS) / (MAX_MOUSE_SENS - MIN_MOUSE_SENS);
}

float COptionsSettings::GetRelativeJoystickSensitivity()
{
	return (JoystickSensitivity - MIN_JOY_SENS) / (MAX_JOY_SENS - MIN_JOY_SENS);
}

float COptionsSettings::GetRelativeJoystickDeadzone()
{
	return (JoystickDeadzone - MIN_JOY_DEAD) / (MAX_JOY_DEAD - MIN_JOY_DEAD);
}

void COptionsSettings::SetRelativeKeyboardSensitivity(float Value)
{
	KeyboardSensitivity = MIN_KEYB_SENS + (Value * (MAX_KEYB_SENS - MIN_KEYB_SENS));
}

void COptionsSettings::SetRelativeMouseSensitivity(float Value)
{
	MouseSensitivity = MIN_MOUSE_SENS + (Value * (MAX_MOUSE_SENS - MIN_MOUSE_SENS));
}

void COptionsSettings::SetRelativeJoystickSensitivity(float Value)
{
	JoystickSensitivity = MIN_JOY_SENS + (Value * (MAX_JOY_SENS - MIN_JOY_SENS));
}

void COptionsSettings::SetRelativeJoystickDeadzone(float Value)
{
	JoystickDeadzone = MIN_JOY_DEAD + (Value * (MAX_JOY_DEAD - MIN_JOY_DEAD));
}

void COptionsSettings::SetDefaultSensitivity()
{
	KeyboardSensitivity = MAX_KEYB_SENS / 2.0f;
	MouseSensitivity = MAX_MOUSE_SENS / 2.0f;
	JoystickSensitivity = MAX_JOY_SENS / 2.0f;
	JoystickDeadzone = MAX_JOY_DEAD / 2.0f;
}
