#include "abstractNodeInterface.h"
#include <QtWidgets/QLayout>
#ifdef DEBUG
#include "debugtrace.h"
#endif

bool abstractNode::_hideCurrent(inframedWidget* replacement)
{
	if (current != untouchable)
		// respects root
	{
		// hides current, and sets focus on replacement or main
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
		// shows replacement. If you want custom focusing, you can place it here
		replacement->show();
		// hiding success
		return true;
	}
	// hiding failure
	return false;
}

void abstractNode::_hideAny(inframedWidget* replacement)
{
	// hides any without respect to root. Is always succesfull
	current->hide();
	current = replacement;
	replacement->setFocus();

	if (replacement == untouchable)
	{
		main->setFocus();
	}
	replacement->show();
}

abstractDynamicNode::abstractDynamicNode(inframedWidget* untouch, QLayout* mLayout)
	: mainLayout(mLayout), currentlyOpened(untouch), untouchable(untouch)
{
	if (untouch && mLayout)
	{
		mLayout->addWidget(untouch);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->setSpacing(0);
	}
}

void abstractDynamicNode::_hideAndDeleteCurrent(inframedWidget* replacement)
{
	// hides and deletes current, but stores root one
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
	// hides and deletes any widget
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