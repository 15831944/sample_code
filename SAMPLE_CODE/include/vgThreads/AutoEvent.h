




#ifndef __AUTOEVENT_H__
#define __AUTOEVENT_H__


#include "event.h"

namespace vgThreads {

	/**
		@date 	2007/12/22  17:30	
		@author  xy
	
		@brief 	
	
		@see    
	*/


	/**
	*	Wrapper around a WIN32 auto event.
	*	Provides an event or signal that automagically resets itself
	*	
	*	@author Peter Hancock
	*
	*/
	class  VGK_EXPORT AutoEvent : public Event
	{
	public:

		/**
		*	Creates the auto event object.
		*	The destructor does not close the handle, the base class Event looks after this.
		*	@param eventName Name of the event can be used for interprocess communication
		*	@throw EventException Thrown when the event cannot be created.
		*/
		AutoEvent() 
		{
			handle = reinterpret_cast<void*>(::CreateEvent(NULL, false, false, NULL));
			if(handle == INVALID_HANDLE_VALUE)
			{
				assert(0);
			}

			DWORD err = GetLastError();
			if ( err != ERROR_SUCCESS )
			{
				assert(0 );
			}
		}

		/**
		*	Signal the event to all threads waiting, and then reset the signal once they've all been informed
		*/
		void signal()
		{
			::PulseEvent(handle);
		}
	};

	
	
	
}// end of namespace vgThreads

#endif // end of __AUTOEVENT_H__


