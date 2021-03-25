#include "Benchmark.h"

#include <Windows.h>
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>

//Initialise static members
RECT Benchmark::m_textRect;
string Benchmark::m_render_engine;
float Benchmark::m_currentFPS = 0;
long Benchmark::m_cpuUsage = 0;
float Benchmark::m_time = 0;
float Benchmark::m_meanFPS = 0;

Benchmark::Benchmark(int run_time, string pc_id, string render_engine, string object_path) {
	m_runTime = run_time;
	m_render_engine = render_engine;
	InitialiseWindow();
	InitialiseFPS();
	InitialiseCPU();
	InitialiseTimer();
	InitialiseLogger(pc_id, GetModelName(object_path), render_engine);
}

Benchmark::~Benchmark() {
	//write logfile
	m_logger->ExportLogFile();
	delete m_logger;

	//CPU
	if (m_canReadCpu)
	{
		PdhCloseQuery(m_queryHandle);
	}
	return;
}


#pragma region Benchmark
/////////////////
/// benchmark ///
/////////////////
bool Benchmark::run()
{
	return (int)m_time < m_runTime;
}

string Benchmark::GetModelName(string model_path) {
	return model_path.substr(model_path.find_last_of('/') + 1,
		model_path.find_last_of('.') - (model_path.find_last_of('/') + 1));
}
#pragma endregion Benchmark


#pragma region Window
//////////////
/// Window ///
//////////////

void Benchmark::InitialiseWindow() {
	CreateDiagWindow(200, 200);
	m_UpdateLastTime = 0;
}

LRESULT CALLBACK Benchmark::WinProcc(HWND hwd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_PAINT: {
		if (!GetUpdateRect(hwd, &m_textRect, FALSE))
			break;    //No regions to update, leave procedure
		UpdateText(hwd);
		break;        //End of procedure
	}
	}

	return DefWindowProc(hwd, msg, wparam, lparam);
}

void Benchmark::CreateDiagWindow(int width, int height) {
	string className = m_render_engine + " - diagnostics";

	//define window style
	WNDCLASS wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WinProcc;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = className.c_str();
	RegisterClass(&wc);
	//create the window
	m_windowHandle = CreateWindow(className.c_str(), className.c_str(), WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 200, 200, width, height, nullptr, nullptr, nullptr, nullptr);
}

HWND Benchmark::getWindowHandle() {
	return m_windowHandle;
}

void Benchmark::UpdateText(HWND hwnd) {

	std::stringstream ss;
	ss << std::fixed << std::setprecision(0)
		<< '\n'
		<< "      " << m_currentFPS << " fps                " << '\n'
		<< "      " << m_meanFPS << " gemiddelde fps        " << '\n'
		<< std::setprecision(2)
		<< "      " << 1000 / m_currentFPS << " ms cpu frametime      " << '\n'
		<< std::setprecision(0)
		<< "      " << m_cpuUsage << " % cpu                " << '\n'
		<< std::setprecision(2)
		<< "      " << m_time << " tijd verstreken          " << '\n';

	std::string diagInfo = ss.str();

	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	GetClientRect(hwnd, &m_textRect);
	DrawText(ps.hdc, diagInfo.c_str(), -1, &m_textRect, DT_LEFT | DT_TOP);
	EndPaint(hwnd, &ps);
}

RECT* Benchmark::getTextRect() {
	return &m_textRect;
}

#pragma endregion window

#pragma region fps
/////////////
///  FPS  ////////////////

void Benchmark::InitialiseFPS() {
	m_FPSCount = 0;
	m_FPSLastTime = 0;
	m_tickPointer = 0;
	m_firstticks = 0;
}


void Benchmark::CalculateFPS() {
	m_FPSCount++;

	if (timeGetTime() >= (m_FPSLastTime + 1000)) //1000 reffers to frames per 'second', we count how many frames are counted in 1 seconds and diplay them every second
	{
		m_currentFPS = m_FPSCount;
		m_FPSCount = 0;

		m_FPSLastTime = timeGetTime();

		CalculateMeanFPS();
	}

	return;
}

void Benchmark::CalculateMeanFPS() {
	// Skip the first 1 fps counts at startup.
	if (m_currentFPS > 1) {

		// Before the first 10 ticks, make the arraysize as big as the data we have.
		int arraySize = (sizeof(m_FPSTicks) / sizeof(*m_FPSTicks));
		int dataSize = 0;
		if (m_firstticks < arraySize) {
			m_firstticks++;
			dataSize = m_firstticks;
		}
		// Else use the initialised arraysize
		else {
			dataSize = arraySize;
		}

		// Set the currentFps at the correct location in the array.
		m_FPSTicks[m_tickPointer] = m_currentFPS;

		// Xount the mean FPS
		float total = 0;
		for (int i = 0; i < dataSize; i++) {
			total += m_FPSTicks[i];
		}
		m_meanFPS = total / dataSize;

		// If The pointer for the data in the array exceeds the size of the array, then point to 0
		if (m_tickPointer >= arraySize) {
			m_tickPointer = 0;
		}
		else {
			m_tickPointer++;
		}

		return;
	}
}


float Benchmark::GetFPS() {
	return m_currentFPS;
}

#pragma endregion fps

#pragma region cpu
/////////////
///  CPU  ///
/////////////

void Benchmark::InitialiseCPU() {
	PDH_STATUS status;

	// Initialize the flag indicating whether this object can read the system cpu usage or not.
	m_canReadCpu = true;

	// Create a query object to poll cpu usage.
	status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	// Set query object to poll all cpus in the system.
	status = PdhAddEnglishCounter(m_queryHandle, "\\Processor(_Total)\\% processor time", 0, &m_counterHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	m_lastSampleTime = timeGetTime();
	m_cpuUsage = 0;
}

void Benchmark::CalculateCPU() {
	PDH_FMT_COUNTERVALUE value;

	if (m_canReadCpu)
	{
		if (timeGetTime() >= (m_lastSampleTime + 1000))
		{
			m_lastSampleTime = timeGetTime();

			PdhCollectQueryData(m_queryHandle);

			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_cpuUsage = value.longValue;
		}
	}
	else
		m_cpuUsage = -1;

	return;
}

int Benchmark::GetCpuPercentage() {
	int usage;

	if (m_canReadCpu)
	{
		usage = (int)m_cpuUsage;
	}
	else
	{
		usage = -1;
	}

	return usage;
}

#pragma endregion cpu

#pragma region timer
/////////////
/// Timer ///
/////////////

using namespace std::chrono;

void Benchmark::InitialiseTimer()
{
	m_lastTimeTimer = std::chrono::steady_clock::now();
}

void Benchmark::UpdateTimer()
{
	m_time = PeekTimer();
}

float Benchmark::MarkTimer() noexcept
{
	const auto old = m_lastTimeTimer;
	m_lastTimeTimer = steady_clock::now();
	const duration<float> frameTime = m_lastTimeTimer - old;
	return frameTime.count();
}

float Benchmark::PeekTimer() const noexcept
{
	return duration<float>(steady_clock::now() - m_lastTimeTimer).count();
}

#pragma endregion timer

#pragma region logger
//////////////
/// logger ///
//////////////
void Benchmark::InitialiseLogger(string pc_id, string object_name, string render_engine)
{
	m_logger = new Logger(render_engine, object_name);
	m_pc_id = pc_id;
	m_object_name = object_name;
}
#pragma endregion logger


void Benchmark::UpdateBenchmark() {
	CalculateFPS();
	auto time = timeGetTime();
	if (time >= (m_UpdateLastTime + 33)) //33 millisecond delay between text updates
	{
		InvalidateRect(getWindowHandle(), getTextRect(), true);
		UpdateTimer();
		CalculateCPU();

		m_UpdateLastTime = time;
	}

	// 1 second delay between log updates
	if (time >= (m_lastLogTime + 1000))
	{
		Log log = Log(time,
			m_pc_id,
			m_render_engine,
			m_object_name,
			m_currentFPS,
			m_cpuUsage);
		m_logger->AddLog(log);
		m_lastLogTime = time;
	}
}