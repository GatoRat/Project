// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <WCL.h>
#include <WHandle.h>
#include <WBuffer.h>
#include <WQueueDefines.h>
#include <WQueue.h>
#include <WCriticalSection.h>

// All objects are OWNED by WPtrQueue while they are queued.
// When the queue is deleted, any remaining items in the queue
// will also be freed. To prevent this, set deleteOnClose to false.

class WPtrQueue
{
private:
	void**   m_pData;
	unsigned m_maxPtrs;
	unsigned m_numPtrs;
	unsigned m_inOffset;
	unsigned m_outOffset;
	bool     m_isOwned;

	WSemaphore	m_filledSlotsSemaphore;
	WEvent		m_closedEvent;

protected:
	WCriticalSection m_postCS;
	WCriticalSection m_popCS;

	virtual void FreePointer(void* p);

public:
	WPtrQueue(unsigned maxPointers = 0, bool isOwned = true);
	virtual ~WPtrQueue()						{ Close(); }

	bool Create(unsigned maxPointers);
	void Close(bool closeQueue = true);

	bool IsCreated() const						{ return m_pData != nullptr; }
	bool IsQueueOpen() const					{ return IsCreated() && !m_closedEvent.IsSet(); }
	long GetMaxLength() const					{ return m_maxPtrs; }
	long GetNumPtrs() const						{ return m_numPtrs; }

	// This only affects behavior when shutting down or reseting and items are still in queue
	bool GetIsOwned() const						{ return m_isOwned; }
	void SetIsOwned(bool isOwned)				{ m_isOwned = isOwned; }

	// Returns:
	// If 0 Success
	// If < 0:
	//   WQUEUE_CLOSED
	//   WQUEUE_BAD_PARAMETER
	//   WQUEUE_DATA_TOO_LONG
	//   WQUEUE_TIMEOUT
	//   WQUEUE_WAIT_FAILED
	//   WQUEUE_SHUTTING_DOWN
	//   WQUEUE_FULL

	int PostItem(void* pItem);

	/////////////////////////////////////////////////////////////////////////////////
	// only the program popping from queue should call the following
	// and there should only be one program popping from the queue

	// If return is nullptr, *pErr is one of the following:
	//   WQUEUE_CLOSED
	//   WQUEUE_TIMEOUT
	//   WQUEUE_WAIT_FAILED
	//   WQUEUE_SHUTTING_DOWN

	void* PopItem(int* pErr = nullptr, DWORD timeout = INFINITE, WEvent* pPriorityHandle1 = nullptr, WEvent* pPriorityHandle2 = nullptr);

	void OpenQueue()							{ m_closedEvent.Reset(); }
	void CloseQueue()							{ m_closedEvent.Set(); }
	void ResetQueue(bool keepClosed = true);

	_WCL_DISALLOW_COPY_AND_ASSIGN(WPtrQueue);
};

///////////////////////////////////////////////////////////////////////////

template <class T>
class WPtrQueueT : public WPtrQueue
{
protected:
	virtual void FreePointer(void* p)								{ delete (T*) p; }

public:
	WPtrQueueT(unsigned maxPointers = 0) : WPtrQueue(maxPointers)	{}

	int PostItem(T* pItem)											{ return WPtrQueue::PostItem(pItem); }
	T* PopItem(int* pErr = nullptr, DWORD timeout = INFINITE, WEvent* pPriorityHandle1 = nullptr, WEvent* pPriorityHandle2 = nullptr)
																	{ return (T*) WPtrQueue::PopItem(pErr, timeout, pPriorityHandle1, pPriorityHandle2); }
	
	_WCL_DISALLOW_COPY_AND_ASSIGN(WPtrQueueT);
};

///////////////////////////////////////////////////////////////////////////
// If the pointer is an allocation of an array use this class
// Alternatively, wrap the array in some base class and use the above

template <class T>
class WPtrArrayQueueT : public WPtrQueue
{
protected:
	virtual void FreePointer(void* p)								{ delete[] (T*)p; }

public:
	WPtrArrayQueueT(unsigned maxPointers = 0) : WPtrQueue(maxPointers)	{}

	int PostItem(T* pItem)											{ return WPtrQueue::PostItem(pItem); }
	T* PopItem(int* pErr = nullptr, DWORD timeout = INFINITE, WEvent* pPriorityHandle1 = nullptr, WEvent* pPriorityHandle2 = nullptr)
																	{ return (T*) WPtrQueue::PopItem(pErr, timeout, pPriorityHandle1, pPriorityHandle2); }
	
	_WCL_DISALLOW_COPY_AND_ASSIGN(WPtrArrayQueueT);
};
