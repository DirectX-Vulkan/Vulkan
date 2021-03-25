#pragma once
#include <Windows.h>

//fps
#pragma comment(lib, "winmm.lib")

//cpu
#pragma comment(lib, "pdh.lib")
#include <pdh.h>

//timer
#include <chrono>

//logger
#include "Logger.h"

using namespace std;

class Benchmark {
public:
	Benchmark(int run_time, string pc_id, string render_engine, string object_path);
	~Benchmark(); //destructor

	//benchmark
	bool run();

	//window
	void InitialiseWindow();
	static LRESULT CALLBACK WinProcc(HWND hwd, UINT msg, WPARAM wparam, LPARAM lparam);
	static void UpdateText(HWND hwnd);
	HWND getWindowHandle();
	RECT* getTextRect();

	//fps
	void InitialiseFPS();
	void CalculateFPS();
	void CalculateMeanFPS();
	float GetFPS();

	//cpu
	void InitialiseCPU();
	void CalculateCPU();
	int GetCpuPercentage();

	//timer
	void InitialiseTimer();
	void UpdateTimer();
	float MarkTimer() noexcept;
	float PeekTimer() const noexcept;

	//logger
	void InitialiseLogger(string pc_id, string object_name, string render_engine);

	void UpdateBenchmark();

private:
	//benchmark
	int m_runTime;
	static string m_render_engine;
	string GetModelName(string model_path);

	//window
	void CreateDiagWindow(int width, int height);
	static RECT m_textRect;
	HWND m_windowHandle;
	float m_UpdateLastTime;

	//fps
	static float m_currentFPS;
	static float m_meanFPS;
	float m_FPSCount;
	float m_FPSLastTime;
	float m_FPSTicks[10];
	int m_tickPointer;
	int m_firstticks;

	//cpu
	static long m_cpuUsage;
	bool m_canReadCpu;
	PDH_HQUERY m_queryHandle;
	PDH_HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;

	//timer
	static float m_time;
	std::chrono::steady_clock::time_point m_lastTimeTimer;

	//logger
	Logger* m_logger = nullptr;
	string m_pc_id;
	string m_object_name;
	float m_lastLogTime;
};