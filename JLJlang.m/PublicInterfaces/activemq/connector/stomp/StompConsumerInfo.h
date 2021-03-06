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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_STOMPCONSUMERINFO_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_STOMPCONSUMERINFO_H_

#include <activemq/connector/ConsumerInfo.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace connector{
namespace stomp{

    class AMQCPP_API StompConsumerInfo : public ConsumerInfo {
    private:

        // Message Selector for this Consumer
        std::string selector;

        // Consumer Id
        long long consumerId;

        // Destination
        cms::Destination* destination;

        // Session Info - We do not own this
        const SessionInfo* session;

        // Consumer shouldn't get messages it publishes
        bool noLocal;

        // Subscription name for this consumer, "" for non-durables
        std::string name;

    public:

        StompConsumerInfo() : ConsumerInfo() {

            selector = "";
            consumerId = 0;
            destination = NULL;
            session = NULL;
            noLocal = false;
        }

        StompConsumerInfo( Connector* connector ) :
            ConsumerInfo( connector ) {

            selector = "";
            consumerId = 0;
            destination = NULL;
            session = NULL;
            noLocal = false;
        }

        virtual ~StompConsumerInfo() {
            this->close();
            delete destination;
        }

        /**
         * Gets this message consumer's message selector expression.
         * @return This Consumer's selector expression or "".
         */
        virtual const std::string& getMessageSelector() const {
            return selector;
        }

        /**
         * Sets this message consumer's message selector expression.
         * @param selector This Consumer's selector expression or "".
         */
        virtual void setMessageSelector( const std::string& selector ) {
            this->selector = selector;
        }

        /**
         * Gets the ID that is assigned to this consumer
         * @return value of the Consumer Id.
         */
        virtual long long getConsumerId() const {
            return consumerId;
        }

        /**
         * Sets the ID that is assigned to this consumer
         * @param id value of the Consumer Id.
         */
        virtual void setConsumerId( long long id ) {
            this->consumerId = id;
        }

        /**
         * Gets the Destination that this Consumer is subscribed on
         * @return Destination this consumer is attached to
         */
        virtual const cms::Destination* getDestination() const {
            return destination;
        }

        /**
         * Sets the destination that this Consumer is listening on
         * @param destination Destination this consumer is attached to
         */
        virtual void setDestination( const cms::Destination* destination ) {
            this->destination = destination->clone();
        }

        /**
         * Gets the Session Info that this consumer is attached too
         * @return SessionnInfo pointer
         */
        virtual const SessionInfo* getSessionInfo() const {
            return session;
        }

        /**
         * Gets the Session Info that this consumer is attached too
         * @param session SessionnInfo for this consumer
         */
        virtual void setSessionInfo( const SessionInfo* session ) {
            this->session = session;
        }

        virtual bool getNoLocal() const {
            return noLocal;
        }

        virtual void setNoLocal( bool noLocal ) {
            this->noLocal = noLocal;
        }

        virtual const std::string& getName() const {
            return name;
        }

        virtual void setName( const std::string& name ) {
            this->name = name;
        }

    };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_STOMPCONSUMERINFO_H_*/
