//----------------------------------------------------------------------------------------------------------------------
// Cyclops project, autonomous RC Car
// Created by Carmelo J. Fernández-Agüera Tortosa (a.k.a. Technik)
// on February 4th, 2012
//----------------------------------------------------------------------------------------------------------------------
// Ultrasound interface

#ifndef _CYCLOPS_HARDWARE_USSENSOR_H_
#define _CYCLOPS_HARDWARE_USSENSOR_H_

namespace hardware
{
	class CUSSensor
	{
	public:
		enum ESensor
		{
			eSensor0,
			eSensor1
		};
		
		enum EState
		{
			eReady,		// ready to measure
			eWaiting,	// Waiting to measure
			eMeasuring,	// Currently measuring distance
		};
		
	public:
		CUSSensor(ESensor _slot);
		void		destroy			();
		int			lastDistance	() const;	// last measured distance (in millimiters)
		void		measure			();
		EState		state			() const;
		void		setCallBack		(void(*_fn)(CUSSensor*));
		
	private:
		~CUSSensor() {}
		
		void		initMeasure			();
		void		listenForEchoSignal	();
		void		listenForEchoEnd	();
		void		finishMeasure		();
		
		static void	onEchoSignal		();
		static void	onEchoEnd			();
		
		static void onCaptureEvent		();
		
	private:
		ESensor			mSlot;
		EState			mState;
		int				mDistance;
		bool			mDestroy;
		void(*mCallback)(CUSSensor*);
		
		static CUSSensor * sActiveSensor;
		static CUSSensor * sNextSensor;
	};
	
	//------------------------------------------------------------------------------------------------------------------
	// Inline implementations
	//------------------------------------------------------------------------------------------------------------------
	inline int CUSSensor::lastDistance() const
	{
		return mDistance;
	}
	
	//------------------------------------------------------------------------------------------------------------------
	inline CUSSensor::EState CUSSensor::state() const
	{
		return mState;
	}
	
	//------------------------------------------------------------------------------------------------------------------
	inline void CUSSensor::setCallBack(void (*_callback)(CUSSensor*))
	{
		mCallback = _callback;
	}
}

#endif // _CYCLOPS_HARDWARE_USSENSOR_H_