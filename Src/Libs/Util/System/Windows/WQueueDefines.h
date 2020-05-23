/**************************************************************************
  © 2002 Joseph Woodbury
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software.
	 
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
	 
  3. This notice may not be removed or altered from any source distribution.
  
  Note to employers of Joseph Woodbury: This software was originally part of
  the CBC or CBC2 libraries and/or derivatives as disclosed in the various
  agreements signed by Joseph Woodbury.
**************************************************************************/
#pragma once

enum
{
	//WQUEUE_WAIT_HANDLE_5	= -105,
	//WQUEUE_WAIT_HANDLE_4	= -104,
	//WQUEUE_WAIT_HANDLE_3	= -103,
	//WQUEUE_WAIT_HANDLE_2	= -102,
	//WQUEUE_WAIT_HANDLE_1	= -101,
	//WQUEUE_WAIT_HANDLE_0	= -100,

	WQUEUE_WAIT_ABANDONED_2	= -13,
	WQUEUE_WAIT_HANDLE_2	= -12,

	WQUEUE_WAIT_ABANDONED_1	= -11,
	WQUEUE_WAIT_HANDLE_1	= -10,

	WQUEUE_BAD_PARAMETER	= -7,
	WQUEUE_DATA_TOO_LONG	= -6,
	WQUEUE_FULL				= -5,
	WQUEUE_TIMEOUT			= -4,
	WQUEUE_CLOSED			= -3,
	WQUEUE_SHUTTING_DOWN	= -2,
	WQUEUE_WAIT_FAILED		= -1,			// can also use WAIT_FAILED as defined by Win32

	WQUEUE_POST_SUCCESS		= 0
};
