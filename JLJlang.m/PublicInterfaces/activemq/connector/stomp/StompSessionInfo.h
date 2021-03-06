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
#ifndef _ACTIVEMQ_CONNECTOR_STOMP_STOMPSESSIONINFO_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_STOMPSESSIONINFO_H_

#include <activemq/util/Config.h>
#include <activemq/connector/SessionInfo.h>
#include <cms/Session.h>
#include <string>

namespace activemq{
namespace connector{
namespace stomp{

    class AMQCPP_API StompSessionInfo : public SessionInfo {
    private:

        // Acknowledge Mode of this Session
        cms::Session::AcknowledgeMode ackMode;

        // The id of the connection to the broker
        // (given to us by the broker)
        std::string connectionId;

        // The unique session id
        long long sessionId;

        // Info for this sessions current transaction
        const TransactionInfo* transaction;

    public:

        StompSessionInfo() : SessionInfo() {

            sessionId = 0;
            ackMode = cms::Session::AUTO_ACKNOWLEDGE;
            transaction = NULL;
        }

        StompSessionInfo( Connector* connector ) :
            SessionInfo( connector ) {

            sessionId = 0;
            ackMode = cms::Session::AUTO_ACKNOWLEDGE;
            transaction = NULL;
        }

        virtual ~StompSessionInfo() {
            this->close();
        }

        /**
         * Gets the Connection Id of the Connection that this consumer is
         * using to receive its messages.
         * @return string value of the connection id
         */
        virtual const std::string& getConnectionId() const{
            return connectionId;
        }

        /**
         * Sets the Connection Id of the Connection that this consumer is
         * using to receive its messages.
         * @param id string value of the connection id
         */
        virtual void setConnectionId( const std::string& id ){
            connectionId = id;
        }

        /**
         * Gets the Sessions Id value
         * @return id for this session
         */
        virtual long long getSessionId() const {
            return sessionId;
        }

        /**
         * Sets the Session Id for this Session
         * @param id integral id value for this session
         */
        virtual void setSessionId( long long id ) {
            this->sessionId = id;
        }

        /**
         * Sets the Ack Mode of this Session Info object
         * @param ackMode Ack Mode
         */
        virtual void setAckMode(cms::Session::AcknowledgeMode ackMode) {
            this->ackMode = ackMode;
        }

        /**
         * Gets the Ack Mode of this Session
         * @return Ack Mode
         */
        virtual cms::Session::AcknowledgeMode getAckMode() const {
            return ackMode;
        }

        /**
         * Gets the currently active transaction info, if this session is
         * transacted, returns NULL when not transacted.  You must call
         * getAckMode and see if the session is transacted.
         * @return Transaction Id of current Transaction
         */
        virtual const TransactionInfo* getTransactionInfo() const {
            return transaction;
        }

        /**
         * Sets the current transaction info for this session, this is nit
         * used when the session is not transacted.
         * @param transaction Transaction Id
         */
        virtual void setTransactionInfo( const TransactionInfo* transaction ) {
            this->transaction = transaction;
        }

   };

}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_STOMPSESSIONINFO_H_*/
