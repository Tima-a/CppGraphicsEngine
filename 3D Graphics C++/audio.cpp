#define MAX_COMMAND_NAME 40
#define AudioLeft 0 
#define AudioRight 1
#define AudioBoth 2
class audio
{
public:
	const char* path = "";
	bool playing = false;
	int speed = 1000;

	//implement current position with deltatime or clock, current position does not work
	audio(const char* audio_path)
	{
		path = audio_path;
	}
	LPCSTR get_file_type()
	{
		int i = search_for_ch(path, '.');
		const char* a = sub_char(path, i+1, get_text_last_index(path));
		return a;
	}
	LPCSTR mcigetstringspeed(static char command_[MAX_COMMAND_NAME], const char* cmnd_name, int speed)
	{
		char command[MAX_COMMAND_NAME] = "";
		append_chars(command, cmnd_name);
		append_chars(command, " \"");
		append_chars(command, path);
		append_chars(command, "\"");
		append_chars(command, " speed ");
		append_chars(command, tocnstch(int(speed)));
		return (LPCSTR)command;

	}
	LPCSTR mcigetstringreverse(static char command_[MAX_COMMAND_NAME], const char* cmnd_name)
	{
		char command[MAX_COMMAND_NAME] = "";
		append_chars(command, cmnd_name);
		append_chars(command, " \"");
		append_chars(command, path);
		append_chars(command, "\"");
		append_chars(command, " reverse");
		return (LPCSTR)command;

	}
	LPCSTR mcigetstring(static char command_[MAX_COMMAND_NAME], const char* cmnd_name, int volume)
	{
		char command[MAX_COMMAND_NAME] = "";
		append_chars(command, cmnd_name);
		append_chars(command, "\"");
		append_chars(command, path);
		append_chars(command, "\"");
		append_chars(command, " volume to ");
		append_chars(command, tocnstch(int(volume)));
		return (LPCSTR)command;

	}
	LPCSTR mcigetstringvol(static char command_[MAX_COMMAND_NAME], const char* cmnd_name, int volume, int audio_channel)
	{
		//mcigetstring(NULL, "open");
		char command[MAX_COMMAND_NAME] = "";
		append_chars(command, cmnd_name);
		append_chars(command, " \"");
		append_chars(command, path);
		append_chars(command, "\"");
		if (audio_channel == AudioRight)
		{
			append_chars(command, " right");
		}
		if (audio_channel == AudioLeft)
		{
			append_chars(command, " left");
		}
		append_chars(command, " volume to ");
		append_chars(command, tocnstch(int(volume)));
		return (LPCSTR)command;

	}
	LPCSTR mcigetstring(static char command_[MAX_COMMAND_NAME], const char* cmnd_name, int pos1, int pos2)
	{
		char command[MAX_COMMAND_NAME] = "";
		append_chars(command, cmnd_name);
		append_chars(command, "\"");
		append_chars(command, path);
		append_chars(command, "\"");
		append_chars(command, " from ");
		append_chars(command, tocnstch(int(pos1)));
		append_chars(command, "to ");
		append_chars(command, tocnstch(int(pos2)));
		return (LPCSTR)command;

	}
	LPCSTR mcigetstring(static char command_[MAX_COMMAND_NAME], const char* cmnd_name)
	{
		char command[MAX_COMMAND_NAME] = "";
		append_chars(command, cmnd_name);
		append_chars(command, " \"");
		append_chars(command, path);
		append_chars(command, "\"");
		if (cmnd_name == "stop")
		{
			append_chars(command, " from position 0");
		}
		return (LPCSTR)command;
	}
	LPCSTR mcigetstringstatus(static char command_[MAX_COMMAND_NAME], const char* cmnd_name, const char* status_cmnd_name)
	{
		char command[MAX_COMMAND_NAME] = "";
		append_chars(command, cmnd_name);
		append_chars(command, " \"");
		append_chars(command, path);
		append_chars(command, "\" ");
		append_chars(command, status_cmnd_name);
		return (LPCSTR)command;
	}
	void play()
	{
		mciSendString(mcigetstring(NULL, "play"), NULL, 0, NULL);
		playing = true;
	}
	void stop()
	{

		mciSendString(mcigetstring(NULL, "stop"), NULL, 0, NULL);
	}
	void resume()
	{

		mciSendString(mcigetstring(NULL, "resume"), NULL, 0, NULL);
	}
	void pause()
	{

		mciSendString(mcigetstring(NULL, "pause"), NULL, 0, NULL);
	}
	void play(int pos1, int pos2)//in miliseconds
	{
		mciSendString(mcigetstring(NULL, "play",pos1,pos2), NULL, 0, NULL);
	}
	void set_volume(int volume_, int audio_channel=AudioBoth)
	{
		mcigetstring(NULL, "open");
		mciSendString(mcigetstringvol(NULL, "setaudio", volume_, audio_channel), NULL, 0, NULL);
	}
	void set_speed(int speed_)
	{
		speed = speed_;
		mciSendString(mcigetstring(NULL, "play"), NULL, 0, NULL);
		mciSendString(mcigetstringspeed(NULL, "set", speed_), NULL, 0, NULL);
	}
	int get_audio_length()
	{
		mciSendString(mcigetstring(NULL,"open"), NULL, 0, NULL);

		char buffer[32];
		mciSendString(mcigetstringstatus(NULL,"status","length"), buffer, sizeof(buffer), NULL);

		return cnstchtoint(buffer);
	}
	int get_current_volume(int audiochannel)
	{
		mciSendString(mcigetstring(NULL, "open"), NULL, 0, NULL);

		char buffer[32];
		if (audiochannel == AudioLeft)
		{
			mciSendString(mcigetstringstatus(NULL, "status", "left volume"), buffer, sizeof(buffer), NULL);
		}
		if (audiochannel == AudioRight)
		{
			mciSendString(mcigetstringstatus(NULL, "status", "right volume"), buffer, sizeof(buffer), NULL);
		}
		if (audiochannel == AudioBoth)
		{
			mciSendString(mcigetstringstatus(NULL, "status", "volume"), buffer, sizeof(buffer), NULL);
		}
		return cnstchtoint(buffer);
	}
	int get_current_treble()
	{
		mciSendString(mcigetstring(NULL, "open"), NULL, 0, NULL);

		char buffer[32];
		mciSendString(mcigetstringstatus(NULL, "status", "treble"), buffer, sizeof(buffer), NULL);
		return cnstchtoint(buffer);
	}
	int get_current_bass()
	{
		mciSendString(mcigetstring(NULL, "open"), NULL, 0, NULL);

		char buffer[32];
		mciSendString(mcigetstringstatus(NULL, "status", "bass"), buffer, sizeof(buffer), NULL);
		return cnstchtoint(buffer);
	}
	float get_current_position()
	{
		return 0;
		//current_position += gtime.delta_time;
	}
};
