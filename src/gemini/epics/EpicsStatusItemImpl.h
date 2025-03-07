/**
 * Implementation of an Epics Status Item.
 * Provides a factory method to instantiate new objects using smart pointers
 */

#ifndef EPICSSTATUSITEMIMPL_H_
#define EPICSSTATUSITEMIMPL_H_

#include <giapi/EpicsStatusItem.h>
#include <giapi/giapi.h>

#include <stdexcept>


namespace giapi {

class EpicsStatusItemImpl: public giapi::EpicsStatusItem {
public:

	const std::string getName() const;

	type::Type getType() const;

	const void * getData() const;

	int getCount() const;

	/**
	 * @throw InvalidOperation
	 * If the EPICS status item does not contain
	 * string data or if the index is out of range.
	 */
	const std::string getDataAsString(int index) const noexcept(false);

	/**
	 * @throw InvalidOperation
	 * If the EPICS status item does not contain
	 * integer data or if the index is out of range.
	 */
	int getDataAsInt(int index) const noexcept(false);

	/**
	 * @throw InvalidOperation
	 * If the EPICS status item does not contain
	 * float data or if the index is out of range.
	 */
	float getDataAsFloat(int index) const noexcept(false);

	/**
	 * @throw InvalidOperation
	 * If the EPICS status item does not contain
	 *        double data or if the index is out of range.
	 */
	double getDataAsDouble(int index) const noexcept(false);

	/**
	 * @throw InvalidOperation
	 * If the EPICS status item does not contain
	 *        byte data or if the index is out of range.
	 */
	unsigned char getDataAsByte(int index) const noexcept(false);



	/**
	 * Create a new EpicsStatusItem via a smart pointer.
	 */
	static pEpicsStatusItem create(const std::string &name,
							type::Type type,
							int count,
							const void * data,
							int size);

	virtual ~EpicsStatusItemImpl();

private:

	/**
	 * Constructor
	 */
	EpicsStatusItemImpl(const std::string &name,
			type::Type type,
			int count,
			const void * data,
			int size);

	/**
	 * Name of the EPICS status item represented by this object
	 */
	std::string _name;

	/**
	 * EPICS status item Data type
	 */
	type::Type _type;

	/**
	 * Number of elements contained in the data section of the
	 * status item. All the elements are of the same type
	 */
	int _nElements;

	/**
	 * Data
	 */
	void * _data;

	/**
	 * Data size
	 */
	int _size;


	/**
	 * Auxiliary method to validate that an index is in the
	 * appropriate range, i.e, 0 <= index < _nElements
	 * @throw InvalidOperation
	 * If the provided index is out of the valid range
	 * (i.e., 0 <= index < _nElements).
	 */
	void validateIndex(int index) const noexcept(false);

};

}

#endif /* EPICSSTATUSITEMIMPL_H_ */
