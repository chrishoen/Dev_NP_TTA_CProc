/*	HLC objects will allow a user to query the SPI bus to get HLC data basck.
 *	Technically, this can be used to bring the SPI bus active and retrieve any
 *	data that happens to flow through, but if it isn't in the context of the
 *	HLC, this will probably just result in errors.
 */

#pragma once

#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <chrono>
#include "smShare.h"

namespace TXRX
{
	class HLC
	{
		public:
			HLC( std::string t_DeviceName )
				: m_DeviceName( t_DeviceName )
			{ }

			std::string getDeviceName()
			{
				return m_DeviceName;
			}

			bool connect();

			void setScaleFactor( double t_ScaleFactor )
			{
				m_ScaleFactor = t_ScaleFactor;
			}

			double getScaleFactor()
			{
				return m_ScaleFactor;
			}

			void setOffset( double t_Offset )
			{
				m_Offset = t_Offset;
			}

			double getOffset()
			{
				return m_Offset;
			}

			unsigned int getRawRFReading();

			double getRFReading()
			{
   			return convertRFValue(getRawRFReading());
			}

			double convertRFValue( unsigned int t_Value )
			{
				return double(t_Value) * m_ScaleFactor + m_Offset;
			}

			bool isConnected()
			{
				return m_Connected;
			}

			bool isReady()
			{
				if (
						!m_Ready
					&&
						(
							std::chrono::steady_clock::now() 
								- m_ReadyWaitTime
						) > m_ReadyWaitTimeout
				)
				{
					m_Ready = true;
				}

				return m_Ready;
			}

			bool isEnabled()
			{
				return m_Enabled;
			}

			bool hadReadError()
			{
				return m_HadReadError;
			}

			void enable()
			{
				m_Enabled = true;
			}

			void disable()
			{
				m_Enabled = false;
			}

			virtual ~ HLC ()
			{
				close( m_DeviceNumber );
			}

		protected:
			unsigned int   m_Speed { 1000000 };
			unsigned short m_Delay { 0 };
			unsigned char  m_WordSize { 16 };
			unsigned char  m_Mode { SPI_CPHA | SPI_CPOL };

			bool m_Enabled { true };
			bool m_Connected { false };
			bool m_Ready { false };
			bool m_HadReadError { false };

			std::chrono::steady_clock::time_point m_ReadyWaitTime;
			std::chrono::milliseconds m_ReadyWaitTimeout { 2500 };

			std::string m_DeviceName;
			int m_DeviceNumber;

			double m_ScaleFactor { -0.025 };	// This is the slope.
			double m_Offset { 22.0 };	//	This is the Y-intercept.

		private:
			HLC() {}
	};

	class HLCSPIDeviceOpenException : public std::exception
	{
		public:
			HLCSPIDeviceOpenException(
				const std::string & t_DeviceName
			) noexcept
				: m_DeviceName( t_DeviceName )
			{ }

			const char* what() const noexcept
			{
				return "Could not open the SPI device for the HLC";
			}
			const std::string m_DeviceName;
	};

	class HLCSPIWriteModeException : public std::exception
	{
		public:
			HLCSPIWriteModeException( const int & t_Mode ) noexcept
				: m_Mode( t_Mode )
			{ }

			const char* what() const noexcept
			{
				return "Could not write SPI mode for HLC communication";
			}
			const int m_Mode;
	};

	class HLCSPIReadModeException : public std::exception
	{
		public:
			HLCSPIReadModeException( const int & t_Mode ) noexcept
				: m_Mode( t_Mode )
			{ }

			const char* what() const noexcept
			{
				return "Could not read SPI mode for HLC communication";
			}
			const int m_Mode;
	};

	class HLCSPIModeMismatchException : public std::exception
	{
		public:
			HLCSPIModeMismatchException(
				const int & t_WriteMode,
				const int & t_ReadMode
			) noexcept
				: m_WriteMode( t_WriteMode ), m_ReadMode( t_ReadMode )
			{ }

			const char* what() const noexcept
			{
				return "The mode that was written to the SPI device does not match the mode that was\nread from the SPI device for HLC communication";
			}
			const int m_WriteMode;
			const int m_ReadMode;
	};

	class HLCSPIWriteWordSizeException : public std::exception
	{
		public:
			HLCSPIWriteWordSizeException( const int & t_WordSize ) noexcept
				: m_WordSize( t_WordSize )
			{ }

			const char* what() const noexcept
			{
				return "Could not write SPI word size for HLC communication";
			}
			const int m_WordSize;
	};

	class HLCSPIReadWordSizeException : public std::exception
	{
		public:
			HLCSPIReadWordSizeException( const int & t_WordSize ) noexcept
				: m_WordSize( t_WordSize )
			{ }

			const char* what() const noexcept
			{
				return "Could not read SPI word size for HLC communication";
			}
			const int m_WordSize;
	};

	class HLCSPIWordSizeMismatchException : public std::exception
	{
		public:
			HLCSPIWordSizeMismatchException(
				const int & t_WriteWordSize,
				const int & t_ReadWordSize
			) noexcept
				: m_WriteWordSize( t_WriteWordSize ), m_ReadWordSize( t_ReadWordSize )
			{ }

			const char* what() const noexcept
			{
				return "The word size that was written to the SPI device does not match the word size\nthat was read from the SPI device for HLC communication";
			}
			const int m_WriteWordSize;
			const int m_ReadWordSize;
	};

	class HLCSPIWriteSpeedException : public std::exception
	{
		public:
			HLCSPIWriteSpeedException( const int & t_Speed ) noexcept
				: m_Speed( t_Speed )
			{ }

			const char* what() const noexcept
			{
				return "Could not write SPI speed for HLC communication";
			}
			const int m_Speed;
	};

	class HLCSPIReadSpeedException : public std::exception
	{
		public:
			HLCSPIReadSpeedException( const int & t_Speed ) noexcept
				: m_Speed( t_Speed )
			{ }

			const char* what() const noexcept
			{
				return "Could not read SPI speed for HLC communication";
			}
			const int m_Speed;
	};

	class HLCSPISpeedMismatchException : public std::exception
	{
		public:
			HLCSPISpeedMismatchException(
				const int & t_WriteSpeed,
				const int & t_ReadSpeed
			) noexcept
				: m_WriteSpeed( t_WriteSpeed ), m_ReadSpeed( t_ReadSpeed )
			{ }

			const char* what() const noexcept
			{
				return "The speed that was written to the SPI device does not match the speed that was\nread from the SPI device for HLC communication";
			}
			const int m_WriteSpeed;
			const int m_ReadSpeed;
	};

	class HLCSPICommunicationException : public std::exception
	{
		public:
			HLCSPICommunicationException() noexcept { }

			const char* what() const noexcept
			{
				return "Got error on attempting to send SPI IOC message";
			}
	};
}
