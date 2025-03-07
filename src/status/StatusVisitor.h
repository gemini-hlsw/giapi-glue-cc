/*
 * StatusVisitor.h
 *
 *  Created on: Jan 20, 2009
 *      Author: anunez
 */

#ifndef STATUSVISITOR_H_
#define STATUSVISITOR_H_

#include <exception>

#include <stdexcept>
namespace giapi {

//forward declarations.
class StatusItem;
class AlarmStatusItem;
class HealthStatusItem;

/**
 * Visitor to apply the visitor pattern over Status Items, allowing
 *  to perform different operations over them.
 */
class StatusVisitor {
public:
	virtual ~StatusVisitor();

	/**
	 * Defines an operation over the StatusItem
	 * @throw std::exception
	 * If an error occurs while processing the status item.
	 */
	virtual void visitStatusItem(StatusItem * item) noexcept(false)=0;

	/**
	 * Defines an operation over the Alarms
	 * @throw std::exception
	 * If an error occurs while processing the alarm status item.
	 */
	virtual void visitAlarmItem(AlarmStatusItem * item) noexcept(false)=0;

	/**
	 * Defines an operation over Health Status item
	 * @throw std::exception
	 * If an error occurs while processing the health status item.
	 */
	virtual void visitHealthItem(HealthStatusItem * item) noexcept(false)=0;

protected:
	StatusVisitor();

};

}

#endif /* STATUSVISITOR_H_ */
