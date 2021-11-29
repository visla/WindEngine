/*
    COptionsSettings.h
	Autor: Tomislav Kukic
	Date: 17.3.2006.
	Version: 1.0    
*/



#ifndef _COptionsSettings_h_
#define _COptionsSettings_h_

#include <CControllerSettings.h>


class COptionsSettings
{
public:
	COptionsSettings();
	~COptionsSettings(){}

	static COptionsSettings* Instance;
	CControllerSettings ControllerSettings;

	int		ResWidth;
	int		ResHeight;
	int		ShaderQuality;
	int		TextureQuality;
	int		PhysicsQuality;
	int		Antialiasing;
	int		Anisotropic;

	bool	PostProcess;
	bool	Particles;
	bool	Glow;
	bool	Shadows;
	bool	Projektors;
	bool	Flares;
	bool	MotionBlur;
	bool	Specular;
	bool	Bumping;
	bool	Refraction;

	float	MusicVolume;
	float	SoundVolume;

	float	KeyboardSensitivity;
	float	MouseSensitivity;
	float	JoystickSensitivity;
	float	JoystickDeadzone;

	void	Save();
	void	Load();

	float	GetRelativeKeyboardSensitivity();
	float	GetRelativeMouseSensitivity();
	float	GetRelativeJoystickSensitivity();
	float	GetRelativeJoystickDeadzone();
	
	void	SetRelativeKeyboardSensitivity(float Value);
	void	SetRelativeMouseSensitivity(float Value);
	void	SetRelativeJoystickSensitivity(float Value);
	void	SetRelativeJoystickDeadzone(float Value);

	void	SetDefaultSettings();
	void	SetDefaultSensitivity();
};




#endif
