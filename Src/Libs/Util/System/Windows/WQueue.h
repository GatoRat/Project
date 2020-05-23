// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <WCL.h>
#include <WHandle.h>
#include <WBuffer.h>
#include <WQueueDefines.h>
#include <WSharedMemory.h>
#include <assert.h>

#ifdef GetFreeSpace
#undef GetFreeSpace
#endif

class WQueueShared
{
protected:
	struct QueueData
	{
		unsigned queueLen;
		unsigned freeSpace;
		unsigned inOffset;
		unsigned outOffset;
		unsigned active;
		unsigned count;
	};

	WSharedMemory	m_sharedMemory;
	QueueData*		m_pQueueData;
	char*			m_pData;
	WMutex			m_postMutex;
	WSemaphore		m_filledSlotsSemaphore;
	WEvent			m_closedEvent;
	bool			m_closeQueueOnDestruction;

private:
	void CopyDataToQueue(const void* pData, int dataLen);
	unsigned CopyDataFromQueue(unsigned datalen, void* pBuffer, unsigned bufferLen);

	int PopData(void* pBuffer, int bufferLen, WBuffer* pData, DWORD timeout, WHandle* pPriorityHandle1, WHandle* pPriorityHandle2);

protected:
	bool Init(bool openOnly, LPCTSTR pPrefix, unsigned totalQueueLen, PSECURITY_ATTRIBUTES pSA, bool forceQueueLen);

public:
	WQueueShared(bool closeQueueOnDestruction = false);
	virtual ~WQueueShared();

	// If forceQueueLen is false, totalQueueLen is the total amount to allocate for the queue, including the size of
	// the QueueData structure. This will be rounded up to the nearest 4k boundary.
	// If forceQueueLen is true, totalQueueLen is the actual length of the queue rounded up to the nearest 4 byte boundary. 4k of shared memory
	// will still be allocated. This should only be used for testing.
	bool Create(LPCTSTR pPrefix, unsigned totalQueueLen, PSECURITY_ATTRIBUTES pSA = nullptr, bool forceQueueLen = false)
															{ return Init(false, pPrefix, totalQueueLen, pSA, forceQueueLen); }
	bool Open(LPCTSTR pPrefix, PSECURITY_ATTRIBUTES pSA = nullptr)
															{ return Init(true, pPrefix, 0, pSA, 0); }

	// If you send a message to the other side with close information, set closeQueue to false.
	void Close(bool closeQueue = true);

	bool IsCreated() const									{ return m_pQueueData != nullptr; }
	bool IsQueueOpen() const								{ return IsCreated() && !m_closedEvent.IsSet(); }
	unsigned GetLength() const								{ return m_pQueueData ? m_pQueueData->queueLen : 0; }
	unsigned GetFreeSpace() const							{ return m_pQueueData ? m_pQueueData->freeSpace : 0; }
	unsigned GetActive() const								{ return m_pQueueData ? m_pQueueData->active : 0; }
	unsigned GetFreePercentage() const						{ return m_pQueueData ? (m_pQueueData->freeSpace * 100U) / m_pQueueData->queueLen : 0; }
	unsigned GetCount() const								{ return m_pQueueData ? m_pQueueData->count : 0; }
	HANDLE   GetClosedEvent() const							{ return m_closedEvent; }

	// Returns:
	// If 0 (WQUEUE_POST_SUCCESS) Success
	// If < 0:
	//   WQUEUE_CLOSED
	//   WQUEUE_BAD_PARAMETER
	//   WQUEUE_DATA_TOO_LONG
	//   WQUEUE_WAIT_HANDLE_1
	//   WQUEUE_WAIT_HANDLE_2
	//   WQUEUE_TIMEOUT
	//   WQUEUE_SHUTTING_DOWN
	//   WQUEUE_WAIT_ABANDONED_1
	//   WQUEUE_WAIT_ABANDONED_2
	//   WQUEUE_WAIT_FAILED
	//   WQUEUE_FULL

	int PostData(const void* pData1, int dataLen1, const void* pData2, int dataLen2, DWORD timeout = 10, WHandle* pPriorityHandle1 = nullptr, WHandle* pPriorityHandle2 = nullptr);
	int PostData(const void* pData, int dataLen, DWORD timeout = 10, WHandle* pPriorityHandle1 = nullptr, WHandle* pPriorityHandle2 = nullptr, const void* pData2 = nullptr, int dataLen2 = 0)
															{ return PostData(pData, dataLen, pData2, dataLen2, timeout, pPriorityHandle1, pPriorityHandle2); }

	/////////////////////////////////////////////////////////////////////////////////
	// only the program popping from queue should call the following
	// and there should only be one program popping from the queue

	// Returns:
	// If >= 0, len copied to buffer
	// If < 0:
	//   WQUEUE_CLOSED
	//   WQUEUE_WAIT_HANDLE_1
	//   WQUEUE_WAIT_HANDLE_2
	//   WQUEUE_TIMEOUT
	//   WQUEUE_SHUTTING_DOWN
	//   WQUEUE_WAIT_ABANDONED_1
	//   WQUEUE_WAIT_ABANDONED_2
	//   WQUEUE_WAIT_FAILED

	int  PopData(void* pBuffer, int bufferLen, DWORD timeout = INFINITE, WHandle* pPriorityHandle1 = nullptr, WHandle* pPriorityHandle2 = nullptr)
															{ return PopData(pBuffer, bufferLen, nullptr, timeout, pPriorityHandle1, pPriorityHandle2); }

	int  PopData(WBuffer& data, DWORD timeout = INFINITE, WHandle* pPriorityHandle1 = nullptr, WHandle* pPriorityHandle2 = nullptr)	
															{ return PopData(nullptr, 0, &data, timeout, pPriorityHandle1, pPriorityHandle2); }
	int  PopData(void* pBuffer, int bufferLen, WBuffer& data, DWORD timeout = INFINITE, WHandle* pPriorityHandle1 = nullptr, WHandle* pPriorityHandle2 = nullptr)
															{ return PopData(pBuffer, bufferLen, &data, timeout, pPriorityHandle1, pPriorityHandle2); }

	void OpenQueue()										{ m_closedEvent.Reset(); }
	void CloseQueue()										{ m_closedEvent.Set(); }
	void ResetQueue(bool keepClosed = true, DWORD timeout = 100);

	/////////////////////////////////////////////////////////////////////////////////
	// These are for debugging purposes

	static int GetOverheadSize()							{ return sizeof(QueueData); }
	unsigned GetInOffset() const							{ return m_pQueueData ? m_pQueueData->inOffset : 0; }
	unsigned GetOutOffset() const							{ return m_pQueueData ? m_pQueueData->outOffset : 0; }

	_WCL_DISALLOW_COPY_AND_ASSIGN(WQueueShared);
};

///////////////////////////////////////////////////////////////////////////

class WQueue : public WQueueShared
{
protected:
	// Open isn't appropriate for non-shared memory
	bool Open()												{ return false; }

public:
	WQueue()												{}
	virtual ~WQueue()										{}

	// unlike above, this is the length of the queue itself
	// it will rounded UP to 16 bytes or the nearest 4 byte boundary, whichever is greater
	bool Create(unsigned queueLen);

	_WCL_DISALLOW_COPY_AND_ASSIGN(WQueue);
};

///////////////////////////////////////////////////////////////////////////

int WQueueWaitFor(WEvent& closeEvent, WHandle& waitHandle, WHandle* pPriorityHandle1, WHandle* pPriorityHandle2, DWORD timeout);
