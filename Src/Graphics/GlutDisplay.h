#pragma once
class GlutDisplay
{
public:
	static GlutDisplay& GetInstance()
	{
		static GlutDisplay instance;
		return instance;
	}
	void Init(int width, int height);
	static void Render();

private:
	GlutDisplay();
	~GlutDisplay();
};

