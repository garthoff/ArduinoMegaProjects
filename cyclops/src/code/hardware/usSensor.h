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
	class USSensor
	{
	public:
		enum ESensor
		{
			eSensor0,
			eSensor1
		};
		
		static unsigned measure(ESensor _sensor);
		
		/*enum EState
		{
			eReady,		// ready to measure
			eWaiting,	// Waiting to measure
			eMeasuring,	// Currently measuring distance
		};
		
	public:
		USSensor(ESensor _slot);
		void		destroy			();
		int			lastDistance	() const;	// last measured distance (in millimiters)
		void		measure			();
		EState		state			() const;
		void		setCallBack		(void(*_fn)(USSensor*));
		
		static void onCaptureEvent		();
	private:
		~USSensor() {}
		
		void		initMeasure			();
		void		listenForEchoSignal	();
		void		listenForEchoEnd	();
		void		finishMeasure		();
		
		static void	onEchoSignal		();
		static void	onEchoEnd			();
		
	private:
		ESensor			mSlot;
		EState			mState;
		int				mDistance;
		bool			mDestroy;
		bool			mEchoArrived;
		unsigned long	mUS;
		void(*mCallback)(USSensor*);
		
		static USSensor * sActiveSensor;
		static USSensor * sNextSensor;*/
	};
	
	//------------------------------------------------------------------------------------------------------------------
	// Inline implementations
	//------------------------------------------------------------------------------------------------------------------
// 	inline int USSensor::lastDistance() const
// 	{
// 		return mDistance;
// 	}
// 	
// 	//------------------------------------------------------------------------------------------------------------------
// 	inline USSensor::EState USSensor::state() const
// 	{
// 		return mState;
// 	}
// 	
// 	//------------------------------------------------------------------------------------------------------------------
// 	inline void USSensor::setCallBack(void (*_callback)(USSensor*))
// 	{
// 		mCallback = _callback;
// 	}
}

#endif // _CYCLOPS_HARDWARE_USSENSOR_H_