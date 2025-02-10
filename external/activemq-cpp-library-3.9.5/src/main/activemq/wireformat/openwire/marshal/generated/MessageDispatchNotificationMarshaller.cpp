/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <activemq/wireformat/openwire/marshal/generated/MessageDispatchNotificationMarshaller.h>

#include <activemq/commands/MessageDispatchNotification.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/lang/Pointer.h>

//
//     NOTE!: This file is autogenerated - do not modify!
//            if you need to make a change, please see the Java Classes in the
//            activemq-core module
//

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::marshal;
using namespace activemq::wireformat::openwire::utils;
using namespace activemq::wireformat::openwire::marshal::generated;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;

///////////////////////////////////////////////////////////////////////////////
DataStructure* MessageDispatchNotificationMarshaller::createObject() const {
    return new MessageDispatchNotification();
}

///////////////////////////////////////////////////////////////////////////////
unsigned char MessageDispatchNotificationMarshaller::getDataStructureType() const {
    return MessageDispatchNotification::ID_MESSAGEDISPATCHNOTIFICATION;
}

///////////////////////////////////////////////////////////////////////////////
void MessageDispatchNotificationMarshaller::tightUnmarshal(OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn, BooleanStream* bs) {

    try {

        BaseCommandMarshaller::tightUnmarshal(wireFormat, dataStructure, dataIn, bs);

        MessageDispatchNotification* info =
            dynamic_cast<MessageDispatchNotification*>(dataStructure);
        info->setConsumerId(Pointer<ConsumerId>(dynamic_cast<ConsumerId* >(
            tightUnmarshalCachedObject(wireFormat, dataIn, bs))));
        info->setDestination(Pointer<ActiveMQDestination>(dynamic_cast<ActiveMQDestination* >(
            tightUnmarshalCachedObject(wireFormat, dataIn, bs))));
        info->setDeliverySequenceId(tightUnmarshalLong(wireFormat, dataIn, bs));
        info->setMessageId(Pointer<MessageId>(dynamic_cast<MessageId* >(
            tightUnmarshalNestedObject(wireFormat, dataIn, bs))));
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

///////////////////////////////////////////////////////////////////////////////
int MessageDispatchNotificationMarshaller::tightMarshal1(OpenWireFormat* wireFormat, DataStructure* dataStructure, BooleanStream* bs) {

    try {

        MessageDispatchNotification* info =
            dynamic_cast<MessageDispatchNotification*>(dataStructure);

        int rc = BaseCommandMarshaller::tightMarshal1(wireFormat, dataStructure, bs);
        rc += tightMarshalCachedObject1(wireFormat, info->getConsumerId().get(), bs);
        rc += tightMarshalCachedObject1(wireFormat, info->getDestination().get(), bs);
        rc += tightMarshalLong1(wireFormat, info->getDeliverySequenceId(), bs);
        rc += tightMarshalNestedObject1(wireFormat, info->getMessageId().get(), bs);

        return rc + 0;
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

///////////////////////////////////////////////////////////////////////////////
void MessageDispatchNotificationMarshaller::tightMarshal2(OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut, BooleanStream* bs) {

    try {

        BaseCommandMarshaller::tightMarshal2(wireFormat, dataStructure, dataOut, bs );

        MessageDispatchNotification* info =
            dynamic_cast<MessageDispatchNotification*>(dataStructure);
        tightMarshalCachedObject2(wireFormat, info->getConsumerId().get(), dataOut, bs);
        tightMarshalCachedObject2(wireFormat, info->getDestination().get(), dataOut, bs);
        tightMarshalLong2(wireFormat, info->getDeliverySequenceId(), dataOut, bs);
        tightMarshalNestedObject2(wireFormat, info->getMessageId().get(), dataOut, bs);
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

///////////////////////////////////////////////////////////////////////////////
void MessageDispatchNotificationMarshaller::looseUnmarshal(OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn) {

    try {

        BaseCommandMarshaller::looseUnmarshal(wireFormat, dataStructure, dataIn);
        MessageDispatchNotification* info =
            dynamic_cast<MessageDispatchNotification*>(dataStructure);
        info->setConsumerId(Pointer<ConsumerId>(dynamic_cast<ConsumerId*>(
            looseUnmarshalCachedObject(wireFormat, dataIn))));
        info->setDestination(Pointer<ActiveMQDestination>(dynamic_cast<ActiveMQDestination*>(
            looseUnmarshalCachedObject(wireFormat, dataIn))));
        info->setDeliverySequenceId(looseUnmarshalLong(wireFormat, dataIn));
        info->setMessageId(Pointer<MessageId>(dynamic_cast<MessageId*>(
            looseUnmarshalNestedObject(wireFormat, dataIn))));
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

///////////////////////////////////////////////////////////////////////////////
void MessageDispatchNotificationMarshaller::looseMarshal(OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut) {

    try {

        MessageDispatchNotification* info =
            dynamic_cast<MessageDispatchNotification*>(dataStructure);
        BaseCommandMarshaller::looseMarshal(wireFormat, dataStructure, dataOut);
        looseMarshalCachedObject(wireFormat, info->getConsumerId().get(), dataOut);
        looseMarshalCachedObject(wireFormat, info->getDestination().get(), dataOut);
        looseMarshalLong(wireFormat, info->getDeliverySequenceId(), dataOut);
        looseMarshalNestedObject(wireFormat, info->getMessageId().get(), dataOut);
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

