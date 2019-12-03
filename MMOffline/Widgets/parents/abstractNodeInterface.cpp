#include "abstractNodeInterface.h"
#include <QtWidgets/QLayout>
#ifdef DEBUG
#include "debugtrace.h"
#endif

bool abstractNode::_hideCurrent(inframedWidget* replacement)
{
	if (current != untouchable)
	{
		current->hide();
		current = replacement;
		if (replacement == untouchable)
		{
			main->setFocus();
		}
		else
		{
			replacement->setFocus();
		}
		replacement->show();
		return true;
	}
	return false;
}

void abstractNode::_hideAny(inframedWidget* replacement)
{
	current->hide();
	current = replacement;
	replacement->setFocus();

	if (replacement == untouchable)
	{
		main->setFocus();
	}
	replacement->show();

#ifdef DEBUG
	detrace_METHEXPL("replacement " << (long long int) replacement << " untch: " << (long long int) untouchable);
#endif
}



abstractDynamicNode::abstractDynamicNode()
{
}

void abstractDynamicNode::_hideAndDeleteCurrent(inframedWidget* replacement)
{
	if (currentlyOpened != untouchable)
	{
		mainLayout->removeWidget(currentlyOpened);
		delete currentlyOpened;
		if (replacement != untouchable)
		{
			mainLayout->addWidget(replacement);
		}
		replacement->show();
		currentlyOpened = replacement;
		currentlyOpened->setFocus();
	}
	else
	{
		delete replacement;
	}
}

void abstractDynamicNode::_hideAnyWithDelete(inframedWidget* replacement)
{
	if (currentlyOpened != untouchable)
	{
		_hideAndDeleteCurrent(replacement);
		return;
	}
	else
	{
		untouchable->hide();
		mainLayout->addWidget(replacement);
		replacement->show();
		currentlyOpened = replacement;
		currentlyOpened->setFocus();
	}
}
