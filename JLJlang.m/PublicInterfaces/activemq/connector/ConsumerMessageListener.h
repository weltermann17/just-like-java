/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _ACTIVEMQ_CONNECTOR_CONSUMERMESSAGELISTENER_H_
#define _ACTIVEMQ_CONNECTOR_CONSUMERMESSAGELISTENER_H_

#include <activemq/util/Config.h>
#include <activemq/connector/ConsumerInfo.h>
#include <activemq/core/ActiveMQMessage.h>

namespace activemq{
namespace connector{

    /**
     * An observer of messages that are targeted at a
     * particular consumer.
     */
    class AMQCPP_API ConsumerMessageListener{
    public:

        virtual ~ConsumerMessageListener() {}

        /**
         * Called to dispatch a message to a particular consumer.
         * @param consumer the target consumer of the dispatch.
         * @param msg the message to be dispatched. the receiver owns this
         * destroy it.
         */
        virtual void onConsumerMessage( ConsumerInfo* consumer,
                                        core::ActiveMQMessage* msg ) = 0;

    };

}}

#endif /*_ACTIVEMQ_CONNECTOR_CONSUMERMESSAGELISTENER_H_*/
