#include "stdafx.h"

#include <bitset>
#include <limits>
#include "HLC.h"

bool TXRX::HLC::connect()
{
	m_DeviceNumber = open( m_DeviceName.c_str(), O_RDWR );
	m_Connected = false;
	if ( m_DeviceNumber < 0 )
	{
		Prn::print(Prn::HLC1, "HLC SPI failure to open device exception %s", m_DeviceName.c_str());
		return false;
	}

	int errVal;

	errVal = ioctl( m_DeviceNumber, SPI_IOC_WR_MODE, &m_Mode );

	if ( errVal == -1 )
	{
		Prn::print(Prn::HLC1, "HLC SPI failure to set mode exception");
		return false;
	}

	char checkMode;
	errVal = ioctl( m_DeviceNumber, SPI_IOC_RD_MODE, &checkMode );

	if ( errVal == -1 )
	{
		Prn::print(Prn::HLC1, "HLC SPI failure to read mode exception");
		return false;
	}

	if ( m_Mode != checkMode )
	{
		Prn::print(Prn::HLC1, "HLC SPI mode mismatch exception");
		return false;
	}

	errVal = ioctl( m_DeviceNumber, SPI_IOC_WR_BITS_PER_WORD, &m_WordSize );

	if ( errVal == -1 )
	{
		Prn::print(Prn::HLC1, "HLC SPI failure to set word size exception");
		return false;
	}

	char checkWordSize;
	errVal = ioctl( m_DeviceNumber, SPI_IOC_RD_BITS_PER_WORD, &checkWordSize );

	if ( errVal == -1 )
	{
		Prn::print(Prn::HLC1, "HLC SPI failure to read word size exception");
		return false;
	}

	if ( m_WordSize != checkWordSize )
	{
		Prn::print(Prn::HLC1, "HLC SPI read word mismatch exception");
		return false;
	}

	errVal = ioctl( m_DeviceNumber, SPI_IOC_WR_MAX_SPEED_HZ, &m_Speed );

	if ( errVal == -1 )
	{
		Prn::print(Prn::HLC1, "HLC SPI failure to set speed exception");
		return false;
	}

	int checkSpeed;
	errVal = ioctl( m_DeviceNumber, SPI_IOC_RD_MAX_SPEED_HZ, &checkSpeed );

	if ( errVal == -1 )
	{
		Prn::print(Prn::HLC1, "HLC SPI failure to read speed exception");
		return false;
	}

	if ( m_Speed != checkSpeed )
	{
		Prn::print(Prn::HLC1, "HLC SPI speed mismatch exception");
		return false;
	}

   m_ReadyWaitTime = std::chrono::steady_clock::now();

	m_Connected = true;

	Prn::print(Prn::HLC1, "HLC SPI connect PASS");
	return true;
}

unsigned int TXRX::HLC::getRawRFReading()
{
	Prn::print(Prn::HLC4, "HLC::getRawRFReading BEGIN");

   //	isReady() evaluates the ready-wate timeout automatically.
	if ( !m_Enabled || !m_Connected || !this->isReady() )
	{
		Prn::print(Prn::HLC1, "HLC::getRawRFReading ERROR 101");
		//	We return the max possible here, because we want a minimal reading
		//	if there is no signal coming through the bus.
		return 8000;
	}

	int errVal;
	m_HadReadError = false;

	spi_ioc_transfer packet = { };
	const int arraySize = 2;	//	We know how many chars we expect to get
						//	back here. Might as well just set it
						//	properly.
	unsigned char response[ arraySize ] = {0};

	packet.tx_buf = (unsigned long)nullptr;
	packet.rx_buf = (unsigned long)response;
	packet.len = arraySize;
	packet.speed_hz = m_Speed;
	packet.bits_per_word = m_WordSize;
	packet.delay_usecs = m_Delay;
	packet.cs_change = 0;

	errVal = ioctl( m_DeviceNumber, SPI_IOC_MESSAGE(1), &packet );
	if ( errVal < 0 )
	{
		Prn::print(Prn::HLC1, "HLC::getRawRFReading ERROR 102");
		m_HadReadError = true;
		return -1;
	}

	//	So, up to this point, it's been pretty straight forward. Now we have
	//	some data, and it is going to be backwards. That is, the data in the
	//	array at index 0 is the SECOND part of the message, and the data at
	//	index 1 is the FIRST part of the message. Also, we need to throw away
	//	the first three bits, and the last bit we get in the total message.
	unsigned short retVal = response[1];
	retVal = retVal << 11;	//	To shift off the first 3 bits.
	retVal = retVal >> 3;	//	Need to fill the top byte.
	retVal = retVal | response[0]; 	//	Should fill in the lower order bytes.
	retVal = retVal >> 1;	//	Should shift off the one garbage bit that we
							//	get at the lowest bit.

	if ( retVal == 0 )
	{
		Prn::print(Prn::HLC1, "HLC::getRawRFReading ERROR 103");
		//	We return the max possible here, because we want a minimal reading
		//	if there is no signal coming through the bus.
		return std::numeric_limits<unsigned short>::max();
	}
	Prn::print(Prn::HLC4, "HLC::getRawRFReading END");

	return retVal;
}
