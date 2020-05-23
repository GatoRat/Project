/**************************************************************************
  © 2000 Joseph Woodbury
  
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
#include "stdafx.h"
#include <WQueuePtr.h>

WPtrQueue::WPtrQueue(unsigned maxPtrs, bool isOwned)
{
	m_maxPtrs =
	m_numPtrs =
	m_inOffset =
	m_outOffset = 0;
	m_isOwned = isOwned;
	m_pData = nullptr;
	if (maxPtrs)
		Create(maxPtrs);
}

void WPtrQueue::FreePointer(void* p)
{
	free(p);
}

void WPtrQueue::Close(bool closeQueue)
{
	if (IsCreated())
	{
		if (closeQueue)
			CloseQueue();

		WCriticalSectionAutoRelease postCS(m_postCS);
		WCriticalSectionAutoRelease popCS(m_popCS);

		m_filledSlotsSemaphore.Close();
		m_closedEvent.Close();

		if (GetIsOwned())
		{
			for (unsigned i = 0; i < m_maxPtrs; i++)
				FreePointer(m_pData[i]);
		}

		free(m_pData);
		m_pData = nullptr;

		m_maxPtrs =
		m_numPtrs =
		m_inOffset =
		m_outOffset = 0;
	}
}

bool WPtrQueue::Create(unsigned maxPtrs)
{
	if (!maxPtrs || IsCreated())
		return false;

	WCriticalSectionAutoRelease postCS(m_postCS);
	WCriticalSectionAutoRelease popCS(m_popCS);

	m_maxPtrs = maxPtrs;
	m_pData = (void**)calloc(maxPtrs, sizeof(void*));

	m_filledSlotsSemaphore.Create(maxPtrs, 0);
	m_closedEvent.Create(TRUE);
	return true;
}

int WPtrQueue::PostItem(void* pItem)
{
	if (!pItem)
		return WQUEUE_BAD_PARAMETER;

	if (!IsQueueOpen())
		return WQUEUE_CLOSED;

	WCriticalSectionAutoRelease postCS(m_postCS);
	WCriticalSectionAutoRelease popCS(m_popCS);

	if (!IsQueueOpen())
		return WQUEUE_CLOSED;

	if (m_numPtrs == m_maxPtrs)
		return WQUEUE_FULL;

	m_pData[m_inOffset++] = pItem;
	if (m_inOffset == m_maxPtrs)
		m_inOffset = 0;

	InterlockedIncrement((long*)&m_numPtrs);
	m_filledSlotsSemaphore.Release(1);
	return WQUEUE_POST_SUCCESS;
}

void* WPtrQueue::PopItem(int* pErr, DWORD timeout, WEvent* pPriorityHandle1, WEvent* pPriorityHandle2)
{
	int err;
	if (!pErr)
		pErr = &err;

	if (!IsQueueOpen())
	{
		*pErr = WQUEUE_CLOSED;
		return nullptr;
	}

	int waitResult = WQueueWaitFor(m_closedEvent, m_filledSlotsSemaphore, pPriorityHandle1, pPriorityHandle2, timeout);
	if (waitResult)
	{
		*pErr = waitResult;
		return nullptr;
	}

	if (!IsQueueOpen())
	{
		*pErr = WQUEUE_CLOSED;
		return nullptr;
	}

	WCriticalSectionAutoRelease popCS(m_popCS);

	if (!m_numPtrs)
	{
		*pErr = waitResult;
		return nullptr;
	}

	void* pItem = m_pData[m_outOffset];
	m_pData[m_outOffset++] = nullptr;
	if (m_outOffset == m_maxPtrs)
		m_outOffset = 0;

#ifdef _WIN32
	InterlockedDecrement(&m_numPtrs);
#else
	__sync_add_and_fetch(&m_numPtrs, 1);
#endif
	return pItem;
}

void WPtrQueue::ResetQueue(bool keepClosed)
{
	bool orgState = m_closedEvent.IsSet();
	m_closedEvent.Set();

	WCriticalSectionAutoRelease postCS(m_postCS);
	WCriticalSectionAutoRelease popCS(m_popCS);

	if (GetIsOwned())
	{
		for (unsigned i = 0; i < m_maxPtrs; i++)
			FreePointer(m_pData[i]);
	}

	m_numPtrs =
	m_inOffset =
	m_outOffset = 0;

	memset(m_pData, 0, m_maxPtrs * sizeof(void*));

	while (m_filledSlotsSemaphore.WaitFor(0) == WAIT_OBJECT_0);

	if (!keepClosed && !orgState)
		m_closedEvent.Reset();
}
