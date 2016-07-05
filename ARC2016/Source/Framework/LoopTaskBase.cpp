#include "pch.h"
#include <chrono>
#include <thread>
#include "Source/Common.h"
#include "LoopTaskBase.h"

using namespace std;
using namespace concurrency;
using namespace ARC2016::Framework::TaskBase;

LoopTaskBase::LoopTaskBase(string taskName, const long cycleMsec)
 : TASK_NAME(taskName)
 , PROC_CYCLE_MSEC(cycleMsec)
 , m_ProcResult(E_TASK_NOT_RUN)
{
	// nop.
}

LoopTaskBase::~LoopTaskBase()
{
	// nop.
}

void LoopTaskBase::Start()
{
	if (IsRunning() == false)
	{
		m_TaskStop = false;
		m_TaskObject = create_task(taskProcedure());
	}
}

void LoopTaskBase::Stop()
{
	if (IsRunning() == true)
	{
		m_TaskStop = true;
		m_TaskObject.then([this](int){});
	}
}

LoopTaskBase::TaskResultEnum LoopTaskBase::GetResult()
{
	return m_ProcResult;
}

bool LoopTaskBase::IsRunning()
{
	bool bRet = false;


	if (m_ProcResult == E_TASK_EXECUTING)
	{
		bRet = true;
	}


	return (bRet);
}

Windows::Foundation::IAsyncOperation<int>^ LoopTaskBase::taskProcedure()
{
	return create_async([this]()
	{
		doProcedure();
		return 0;
	});
}

void LoopTaskBase::doProcedure()
{
	ResultEnum	result = E_RET_ABNORMAL;


	// �^�X�N���쒆
	m_ProcResult = E_TASK_EXECUTING;

	// ��������
	result = initialize();
	if (result != E_RET_NORMAL)
	{
		m_ProcResult = E_TASK_ABNORMAL_END;
		goto FINISH;
	}

	// ���C�����[�v
	while (m_TaskStop == false)
	{
		result = taskMain();
		if (result != E_RET_NORMAL)
		{
			m_ProcResult = E_TASK_ABNORMAL_END;
			goto FINISH;
		}
		
		this_thread::sleep_for(std::chrono::milliseconds(PROC_CYCLE_MSEC));
	}

	// �I������
	result = finalize();
	if (result != E_RET_NORMAL)
	{
		m_ProcResult = E_TASK_ABNORMAL_END;
		goto FINISH;
	}

	m_ProcResult = E_TASK_NORMAL_END;


FINISH:
	return;
}