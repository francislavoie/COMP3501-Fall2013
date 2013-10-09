///////////////////////////////////////////////////////////////////////////////
// Filename: cpuclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "cpu.h"


Cpu::Cpu() { }


Cpu::Cpu(const Cpu& other) { }


Cpu::~Cpu() { }


void Cpu::Initialize() {
	PDH_STATUS status;

	// Initialize the flag indicating whether this object can read the system cpu usage or not.
	m_canReadCpu = true;

	// Create a query object to poll cpu usage.
	status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if(status != ERROR_SUCCESS) m_canReadCpu = false;

	// Set query object to poll all cpus in the system.
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if(status != ERROR_SUCCESS) m_canReadCpu = false;

	m_lastSampleTime = GetTickCount(); 

	m_cpuUsage = 0;

	return;
}


void Cpu::Shutdown() {
	if(m_canReadCpu)
		PdhCloseQuery(m_queryHandle);

	return;
}


void Cpu::Frame() {
	PDH_FMT_COUNTERVALUE value; 

	if(m_canReadCpu) {
		if((m_lastSampleTime + 1000) < GetTickCount()) {
			m_lastSampleTime = GetTickCount(); 

			PdhCollectQueryData(m_queryHandle);
		
			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_cpuUsage = value.longValue;
		}
	}

	return;
}


int Cpu::GetCpuPercentage() {
	int usage;

	if(m_canReadCpu)
		usage = (int)m_cpuUsage;
	else
		usage = 0;

	return usage;
}