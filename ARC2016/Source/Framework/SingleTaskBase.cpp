#include "pch.h"
#include "SingleTaskBase.h"

using namespace concurrency;
using namespace ARC2016::Framework::TaskBase;

SingleTaskBase::SingleTaskBase()
 : m_ProcResult(E_TASK_NOT_RUN)
{
	// nop.
}

SingleTaskBase::~SingleTaskBase()
{
	// nop.
}

void SingleTaskBase::Start()
{
	if (IsRunning() == false)
	{
		m_TaskObject = create_task(taskProcedure());
	}
}

bool SingleTaskBase::IsRunning()
{
	bool	bRet = false;

	if (m_ProcResult == E_TASK_EXECUTING)
	{
		bRet = true;
	}

	return (bRet);
}

SingleTaskBase::TaskResultEnum SingleTaskBase::GetResult()
{
	return m_ProcResult;
}

Windows::Foundation::IAsyncOperation<int>^ SingleTaskBase::taskProcedure()
{
	return create_async([this]()
	{
		doProcedure();
		return 0;
	});
}

void SingleTaskBase::doProcedure()
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

	// ���C������
	result = taskMain();
	if (result != E_RET_NORMAL)
	{
		m_ProcResult = E_TASK_ABNORMAL_END;
		goto FINISH;
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
