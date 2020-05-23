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
#include <WQueue.h>
#include <WPath.h>
#include <WStr.h>

WQueueShared::WQueueShared(bool closeQueueOnDestruction)
	: m_pQueueData(nullptr)
	, m_pData(nullptr)
	, m_closeQueueOnDestruction(closeQueueOnDestruction)
{
}

WQueueShared::~WQueueShared()
{ 
	Close(m_closeQueueOnDestruction);
}

void WQueueShared::Close(bool closeQueue)
{
	if (IsCreated())
	{
		if (closeQueue)
			CloseQueue();
		m_postMutex.WaitFor(100);
		m_filledSlotsSemaphore.Close();
		m_sharedMemory.Free();
		m_pQueueData = nullptr;
		m_pData = nullptr;
		m_postMutex.Close();
		m_closedEvent.Close();
	}
}

bool WQueueShared::Init(bool openOnly, LPCTSTR pPrefix, unsigned totalQueueLen, PSECURITY_ATTRIBUTES pSA, bool forceQueueLen)
{
	if (IsCreated())
		return false;

	if (!pSA)
		pSA = &wNullSecurityAttribute;

	TCHAR name[MAX_PATH];
	WMakeObjectName(name, pPrefix, WCL_MUTEX, pPrefix ? _T("") : nullptr, WCL_PATH_APPNAMEONLY);

#ifdef _WIN32_WCE
	m_postMutex.Create(!openOnly, pPrefix ? name : nullptr, pSA);
#else
	if (openOnly)
		m_postMutex.Open(pPrefix ? name : nullptr);
	else
		m_postMutex.Create(TRUE, pPrefix ? name : nullptr, pSA);
#endif

	if (m_postMutex.IsOpen())
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS || openOnly)
		{
			openOnly = true;
			m_postMutex.WaitFor(500);
		}

		unsigned length;

		if (forceQueueLen && totalQueueLen)
		{
			totalQueueLen = (totalQueueLen + 3) & ~3;
			length = GetOverheadSize() + totalQueueLen;
		}
		else
		{
			length = totalQueueLen ? (totalQueueLen + 1023) & ~1023 : 1024;
		}
		
		WMakeObjectName(name, pPrefix, WCL_MEMORY, pPrefix ? _T("") : nullptr, WCL_PATH_APPNAMEONLY);

		m_pQueueData = (QueueData*) m_sharedMemory.Alloc(name, length, openOnly, pSA);

		if (!m_pQueueData)
		{
			m_postMutex.Release();
			m_postMutex.Close();
			return false;
		}

		m_pData = (char*) &m_pQueueData[1];

		WMakeObjectName(name, pPrefix, WCL_SEMAPHORE, pPrefix ? _T("") : nullptr, WCL_PATH_APPNAMEONLY);

		TCHAR name2[MAX_PATH];
		WMakeObjectName(name2, pPrefix, WCL_EVENT, pPrefix ? _T("") : nullptr, WCL_PATH_APPNAMEONLY);

		if (openOnly)
		{
			m_filledSlotsSemaphore.Open(name);
			m_closedEvent.Open(name2);
		}
		else
		{
			memset(m_pQueueData, 0, length);
			m_pQueueData->freeSpace = 
			m_pQueueData->queueLen = forceQueueLen ? totalQueueLen : length - sizeof(QueueData);

			m_filledSlotsSemaphore.Create(m_pQueueData->queueLen / sizeof(unsigned), 0, name, pSA);
			m_closedEvent.Create(TRUE, FALSE, name2, pSA);
		}
		
		m_postMutex.Release();
	}

	return true;
}

void WQueueShared::CopyDataToQueue(const void* pData, int dataLen)
{
	if (pData && dataLen > 0)
	{
		unsigned remainderLen = GetLength() - m_pQueueData->inOffset;
		if ((unsigned) dataLen > remainderLen)
		{
			if (remainderLen > 0)
				memcpy(&m_pData[m_pQueueData->inOffset], pData, remainderLen);

			memcpy(m_pData, (char*)pData + remainderLen, dataLen - remainderLen);

			m_pQueueData->inOffset = dataLen - remainderLen;
		}
		else
		{
			memcpy(&m_pData[m_pQueueData->inOffset], pData, dataLen);
			m_pQueueData->inOffset += dataLen;
		}
	}
}

int WQueueShared::PostData(const void* pData1, int dataLen1, const void* pData2, int dataLen2, DWORD timeout, WHandle* pPriorityHandle1, WHandle* pPriorityHandle2)
{
	if (!IsQueueOpen())
		return WQUEUE_CLOSED;

	if (!pData1 && !pData2)
		return WQUEUE_BAD_PARAMETER;

	unsigned totalDataLen = (dataLen1 >= 0 ? dataLen1 : 0) + (dataLen2 >= 0 ? dataLen2 : 0);
	if (!totalDataLen)
		return WQUEUE_BAD_PARAMETER;

	if ((unsigned)(sizeof(unsigned) + totalDataLen) > GetLength())
		return WQUEUE_DATA_TOO_LONG;

	int waitResult = WQueueWaitFor(m_closedEvent, m_postMutex, pPriorityHandle1, pPriorityHandle2, timeout);
	if (waitResult)
		return waitResult;

	if (!IsQueueOpen())
	{
		m_postMutex.Release();
		return WQUEUE_CLOSED;
	}

	if ((unsigned)(sizeof(unsigned) + totalDataLen) > m_pQueueData->freeSpace)
	{
		m_postMutex.Release();
		return WQUEUE_FULL;
	}

	*(unsigned*)&m_pData[m_pQueueData->inOffset] = (unsigned) totalDataLen;
	m_pQueueData->inOffset += sizeof(unsigned);

	CopyDataToQueue(pData1, dataLen1);
	CopyDataToQueue(pData2, dataLen2);

	if (m_pQueueData->inOffset & 3)
	{
		unsigned extra = 4 - (m_pQueueData->inOffset & 3);
		totalDataLen += extra;
		m_pQueueData->inOffset += extra;
	}

	if (m_pQueueData->inOffset == GetLength())
		m_pQueueData->inOffset = 0;

	m_pQueueData->count++;
	InterlockedExchangeAdd((long*)&m_pQueueData->freeSpace, -(long)(sizeof(unsigned) + totalDataLen));
	InterlockedIncrement((long*)&m_pQueueData->active);

	m_filledSlotsSemaphore.Release();
	
	m_postMutex.Release();
	return WQUEUE_POST_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////

unsigned WQueueShared::CopyDataFromQueue(unsigned dataLen, void* pBuffer, unsigned bufferLen)
{
	unsigned remainderLen = GetLength() - m_pQueueData->outOffset;
	if (dataLen > remainderLen)
	{
		if (pBuffer && bufferLen)
		{
			if (remainderLen)
			{
				if ((unsigned) bufferLen <= remainderLen)
				{
					memcpy(pBuffer, &m_pData[m_pQueueData->outOffset], bufferLen);
				}
				else
				{
					memcpy(pBuffer, &m_pData[m_pQueueData->outOffset], remainderLen);
					memcpy((char*)pBuffer + remainderLen, m_pData, bufferLen - remainderLen);
				}
			}
			else
			{
				memcpy(pBuffer, m_pData, bufferLen);
			}
		}
		m_pQueueData->outOffset = dataLen - remainderLen;
	}
	else
	{
		if (pBuffer && bufferLen)
			memcpy(pBuffer, &m_pData[m_pQueueData->outOffset], bufferLen);

		m_pQueueData->outOffset += dataLen;
	}
	return bufferLen;
}

int WQueueShared::PopData(void* pBuffer, int bufferLen, WBuffer* pWBuffer, DWORD timeout, WHandle* pPriorityHandle1, WHandle* pPriorityHandle2)
{
	if (pWBuffer)
		pWBuffer->SetLength(0);

	if (!IsQueueOpen())
		return WQUEUE_CLOSED;

	assert(bufferLen >= 0);
	if (bufferLen < 0)
		bufferLen = 0;

	int waitResult = WQueueWaitFor(m_closedEvent, m_filledSlotsSemaphore, pPriorityHandle1, pPriorityHandle2, timeout);
	if (waitResult)
		return waitResult;

	if (!IsQueueOpen())
		return WQUEUE_CLOSED;

	unsigned dataLen = *(unsigned*)&m_pData[m_pQueueData->outOffset];
	m_pQueueData->outOffset += sizeof(unsigned);

	unsigned dataCopied = 0;

	if (pWBuffer && (unsigned) bufferLen < dataLen)
	{
		if (bufferLen > 0)
			dataCopied = CopyDataFromQueue(bufferLen, pBuffer, bufferLen);

		pWBuffer->SetLength(dataLen - dataCopied);
		dataCopied += CopyDataFromQueue(pWBuffer->GetLength(), pWBuffer->GetAddress(), pWBuffer->GetLength());
	}
	else
	{
		dataCopied = CopyDataFromQueue(dataLen, pBuffer, (unsigned) bufferLen > dataLen ? dataLen : bufferLen);
	}

	if (m_pQueueData->outOffset & 3)
	{
		int extra = 4 - (m_pQueueData->outOffset & 3);
		dataLen += extra;
		m_pQueueData->outOffset += extra;
	}

	if (m_pQueueData->outOffset == GetLength())
		m_pQueueData->outOffset = 0;

	InterlockedExchangeAdd((long*)&m_pQueueData->freeSpace, dataLen + sizeof(unsigned));
	InterlockedDecrement((long*)&m_pQueueData->active);

	return (int) dataCopied;
}

void WQueueShared::ResetQueue(bool keepClosed, DWORD timeout)
{
	assert(m_pQueueData);

	m_postMutex.WaitFor(timeout);
	bool orgState = m_closedEvent.IsSet();
	m_closedEvent.Set();

	m_pQueueData->inOffset = 
	m_pQueueData->outOffset = 0;
	m_pQueueData->freeSpace = m_pQueueData->queueLen;
	m_pQueueData->active = 0;

	while (m_filledSlotsSemaphore.WaitFor(0) == WAIT_OBJECT_0);

	m_postMutex.Release();
	if (!keepClosed && !orgState)
		m_closedEvent.Reset();
}

///////////////////////////////////////////////////////////////////////////

bool WQueue::Create(unsigned queueLen)
{
	if (!IsQueueOpen())
	{
		m_postMutex.Create();
		if (m_postMutex.IsOpen())
		{
			queueLen = queueLen <= 16 ? 16 : (queueLen + 3) & ~3;
			unsigned length = GetOverheadSize() + queueLen;
			m_pQueueData = (QueueData*) m_sharedMemory.Alloc(nullptr, length, false, nullptr);

			if (!m_pQueueData)
			{
				m_postMutex.Release();
				m_postMutex.Close();
				return false;
			}

			m_pData = (char*) &m_pQueueData[1];

			memset(m_pQueueData, 0, length);
			m_pQueueData->freeSpace = 
			m_pQueueData->queueLen = queueLen;

			m_filledSlotsSemaphore.Create(m_pQueueData->queueLen / sizeof(unsigned), 0);
			m_closedEvent.Create(TRUE, FALSE);
			m_postMutex.Release();
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////

int WQueueWaitFor(WEvent& closeEvent, WHandle& waitHandle, WHandle* pPriorityHandle1, WHandle* pPriorityHandle2, DWORD timeout)
{
	DWORD numHandles = 0;
	HANDLE handles[4];

	DWORD object0;
	if (pPriorityHandle1)
	{
		object0 = numHandles;
		handles[numHandles++] = pPriorityHandle1->GetHandle();
	}
	else
	{
		object0 = (DWORD) -1;
	}

	DWORD object1;
	if (pPriorityHandle2)
	{
		object1 = numHandles;
		handles[numHandles++] = pPriorityHandle2->GetHandle();
	}
	else
	{
		object1 = (DWORD) -1;
	}

	DWORD closeObject = numHandles;
	handles[numHandles++] = closeEvent.GetHandle();

	DWORD waitObject = numHandles;
	handles[numHandles++] = waitHandle.GetHandle();

	DWORD result = ::WaitForMultipleObjects(numHandles, handles, FALSE, timeout);
	if (result == closeObject)
		return WQUEUE_CLOSED;

	if (result == waitObject)
		return 0;

	if (result == object0)
		return WQUEUE_WAIT_HANDLE_1;

	if (result == object1)
		return WQUEUE_WAIT_HANDLE_2;

	if (result == WAIT_TIMEOUT)
		return WQUEUE_TIMEOUT;

	if (result == WAIT_ABANDONED_0 + waitObject)
		return WQUEUE_SHUTTING_DOWN; // Only the posting application would cause this to be set; this should only happen if a second thread deleted the posting thread at exactly during the wait

	if (result == WAIT_ABANDONED_0 + object0)
		return WQUEUE_WAIT_ABANDONED_1;

	if (result == WAIT_ABANDONED_0 + object1)
		return WQUEUE_WAIT_ABANDONED_2;

	return WQUEUE_WAIT_FAILED;
}
