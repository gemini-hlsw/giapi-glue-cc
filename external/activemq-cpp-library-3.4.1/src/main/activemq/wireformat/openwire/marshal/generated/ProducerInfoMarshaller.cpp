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

#include <activemq/wireformat/openwire/marshal/generated/ProducerInfoMarshaller.h>

#include <activemq/commands/ProducerInfo.h>
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
DataStructure* ProducerInfoMarshaller::createObject() const {
    return new ProducerInfo();
}

///////////////////////////////////////////////////////////////////////////////
unsigned char ProducerInfoMarshaller::getDataStructureType() const {
    return ProducerInfo::ID_PRODUCERINFO;
}

///////////////////////////////////////////////////////////////////////////////
void ProducerInfoMarshaller::tightUnmarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn, BooleanStream* bs ) {

    try {

        BaseCommandMarshaller::tightUnmarshal( wireFormat, dataStructure, dataIn, bs );

        ProducerInfo* info =
            dynamic_cast<ProducerInfo*>( dataStructure );

        int wireVersion = wireFormat->getVersion();

        info->setProducerId( Pointer<ProducerId>( dynamic_cast< ProducerId* >(
            tightUnmarshalCachedObject( wireFormat, dataIn, bs ) ) ) );
        info->setDestination( Pointer<ActiveMQDestination>( dynamic_cast< ActiveMQDestination* >(
            tightUnmarshalCachedObject( wireFormat, dataIn, bs ) ) ) );

        if( bs->readBoolean() ) {
            short size = dataIn->readShort();
            info->getBrokerPath().reserve( size );
            for( int i = 0; i < size; i++ ) {
                info->getBrokerPath().push_back( Pointer<BrokerId>( dynamic_cast< BrokerId* >(
                    tightUnmarshalNestedObject( wireFormat, dataIn, bs ) ) ) );
            }
        } else {
            info->getBrokerPath().clear();
        }
        if( wireVersion >= 2 ) {
            info->setDispatchAsync( bs->readBoolean() );
        }
        if( wireVersion >= 3 ) {
            info->setWindowSize( dataIn->readInt() );
        }
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

///////////////////////////////////////////////////////////////////////////////
int ProducerInfoMarshaller::tightMarshal1( OpenWireFormat* wireFormat, DataStructure* dataStructure, BooleanStream* bs ) {

    try {

        ProducerInfo* info =
            dynamic_cast<ProducerInfo*>( dataStructure );

        int rc = BaseCommandMarshaller::tightMarshal1( wireFormat, dataStructure, bs );

        int wireVersion = wireFormat->getVersion();

        rc += tightMarshalCachedObject1( wireFormat, info->getProducerId().get(), bs );
        rc += tightMarshalCachedObject1( wireFormat, info->getDestination().get(), bs );
        rc += tightMarshalObjectArray1( wireFormat, info->getBrokerPath(), bs );
        if( wireVersion >= 2 ) {
            bs->writeBoolean( info->isDispatchAsync() );
        }
        if( wireVersion >= 3 ) {
        }

        return rc + 4;
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

///////////////////////////////////////////////////////////////////////////////
void ProducerInfoMarshaller::tightMarshal2( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut, BooleanStream* bs ) {

    try {

        BaseCommandMarshaller::tightMarshal2( wireFormat, dataStructure, dataOut, bs );

        ProducerInfo* info =
            dynamic_cast<ProducerInfo*>( dataStructure );

        int wireVersion = wireFormat->getVersion();

        tightMarshalCachedObject2( wireFormat, info->getProducerId().get(), dataOut, bs );
        tightMarshalCachedObject2( wireFormat, info->getDestination().get(), dataOut, bs );
        tightMarshalObjectArray2( wireFormat, info->getBrokerPath(), dataOut, bs );
        if( wireVersion >= 2 ) {
            bs->readBoolean();
        }
        if( wireVersion >= 3 ) {
            dataOut->writeInt( info->getWindowSize() );
        }
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

///////////////////////////////////////////////////////////////////////////////
void ProducerInfoMarshaller::looseUnmarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn ) {

    try {

        BaseCommandMarshaller::looseUnmarshal( wireFormat, dataStructure, dataIn );
        ProducerInfo* info =
            dynamic_cast<ProducerInfo*>( dataStructure );

        int wireVersion = wireFormat->getVersion();

        info->setProducerId( Pointer<ProducerId>( dynamic_cast< ProducerId* >( 
            looseUnmarshalCachedObject( wireFormat, dataIn ) ) ) );
        info->setDestination( Pointer<ActiveMQDestination>( dynamic_cast< ActiveMQDestination* >( 
            looseUnmarshalCachedObject( wireFormat, dataIn ) ) ) );

        if( dataIn->readBoolean() ) {
            short size = dataIn->readShort();
            info->getBrokerPath().reserve( size );
            for( int i = 0; i < size; i++ ) {
                info->getBrokerPath().push_back( Pointer<BrokerId>( dynamic_cast<BrokerId* >(
                    looseUnmarshalNestedObject( wireFormat, dataIn ) ) ) );
            }
        } else {
            info->getBrokerPath().clear();
        }
        if( wireVersion >= 2 ) {
            info->setDispatchAsync( dataIn->readBoolean() );
        }
        if( wireVersion >= 3 ) {
            info->setWindowSize( dataIn->readInt() );
        }
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

///////////////////////////////////////////////////////////////////////////////
void ProducerInfoMarshaller::looseMarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut ) {

    try {

        ProducerInfo* info =
            dynamic_cast<ProducerInfo*>( dataStructure );
        BaseCommandMarshaller::looseMarshal( wireFormat, dataStructure, dataOut );

        int wireVersion = wireFormat->getVersion();

        looseMarshalCachedObject( wireFormat, info->getProducerId().get(), dataOut );
        looseMarshalCachedObject( wireFormat, info->getDestination().get(), dataOut );
        looseMarshalObjectArray( wireFormat, info->getBrokerPath(), dataOut );
        if( wireVersion >= 2 ) {
            dataOut->writeBoolean( info->isDispatchAsync() );
        }
        if( wireVersion >= 3 ) {
            dataOut->writeInt( info->getWindowSize() );
        }
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

