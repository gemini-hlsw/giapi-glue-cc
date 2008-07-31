#ifndef JMSCOMMANDUTIL_H_
#define JMSCOMMANDUTIL_H_


#include <log4cxx/logger.h>
#include <giapi/giapi.h>
#include <giapi/SequenceCommandHandler.h>
#include <giapi/HandlerResponse.h>

#include <gmp/SequenceCommandConsumer.h>

#include <util/giapiMaps.h>


namespace giapi {


/**
 * The ActivityHolder class is a container that is used to keep track
 * of the different <code>gmp::SequenceCommandConsumer</code> objects
 * associated to each <code>command::Activity</code> element.
 * 
 * The ActivityHolder is in charge of having a <i>unique</i> 
 * <code>gmp::SequenceCommandConsumer</code> for each
 * <code>command::Activity</code>. If a new consumer is registered
 * for an Activity, the old one is discarded (and deleted
 * if nobody else is using it) 
 * 
 * The CommandUtil class associates an <code>ActivityHolder</code> to 
 * each <code>command::SequenceCommand</code>. 
 */
class ActivityHolder {
	/**
	 * Logging facility
	 */
	static log4cxx::LoggerPtr logger;

public:

	ActivityHolder();

	/**
	 * Associate the <code>consumer</code> to the activites
	 * represented by the <code>set</code> argument. 
	 * 
	 * @param set the set of <code>Activity</code> elements
	 * that will be associated with the consumer
	 * @param consumer the sequence command consumer that
	 * will be associated to the activities. If an activity
	 * was already associated to a consumer, then that association
	 * will be removed. The most recent sequence command consumer
	 * registered is the one that will be used. 
	 */
	void registerConsumer(command::ActivitySet set,
			gmp::pSequenceCommandConsumer consumer);

	/**
	 * Associate the <code>consumer</code> to the 
	 * <code>activity</code>.
	 * 
	 * @param activity the  <code>Activity</code> 
	 * that will be associated with the consumer
	 * @param consumer the sequence command consumer associated
	 * to the activity. If an existing association exists, 
	 * it will be discarded. The most recent consumer registered
	 * will be used in all the cases
	 */

	void registerConsumer(command::Activity activity,
			gmp::pSequenceCommandConsumer consumer);

	/**
	 * Destructor. Removes all the associations, releasing 
	 * any resources allocated.
	 */
	virtual ~ActivityHolder();

private:
	/**
	 * Type definition for the hash_table that will map command Ids to 
	 * the JMS Topic associated to them.
	 */
	typedef hash_map<command::Activity, gmp::pSequenceCommandConsumer>
			ActivityConsumerMap;

	ActivityConsumerMap _activityConsumerMap;
};

class JmsCommandUtil {

	/**
	 * Logging facility
	 */
	static log4cxx::LoggerPtr logger;

public:

	int subscribeSequenceCommand(command::SequenceCommand id,
			command::ActivitySet activities, pSequenceCommandHandler handler);

	int subscribeApply(const char* prefix, command::ActivitySet activities,
			pSequenceCommandHandler handler);

	int postCompletionInfo(command::ActionId id, pHandlerResponse response);

	static JmsCommandUtil& Instance();

	virtual ~JmsCommandUtil();

private:
	/**
	 * Internal instance of this utility class
	 */
	static std::auto_ptr<JmsCommandUtil> INSTANCE;
	JmsCommandUtil();

	typedef hash_map<command::SequenceCommand, ActivityHolder *>
			CommandHolderMap;

	CommandHolderMap _commandHolderMap;

};

}

#endif /*JMSCOMMANDUTIL_H_*/