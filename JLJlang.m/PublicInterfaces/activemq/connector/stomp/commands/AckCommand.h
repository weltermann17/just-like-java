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

#ifndef _ACTIVEMQ_CONNCETOR_STOMP_COMMANDS_ACKCOMMAND_H_
#define _ACTIVEMQ_CONNCETOR_STOMP_COMMANDS_ACKCOMMAND_H_

#include <activemq/util/Config.h>
#include <activemq/connector/stomp/commands/AbstractCommand.h>
#include <activemq/connector/stomp/commands/CommandConstants.h>
#include <activemq/transport/Command.h>

namespace activemq{
namespace connector{
namespace stomp{
namespace commands{

    /**
     * Stomp Command that Represents Acknowledgment of a message
     * receive.  The Ack Command has one required attribute, message
     * Id.  For each message sent to the client from the broker, the
     * message will not be considered consumed until an Ack is sent.
     * Optionally a Transaction Id can be set that indicates that the
     * message acknowledgment should be part of a named transaction.
     */
    class AMQCPP_API AckCommand : public AbstractCommand< transport::Command > {
    public:

        AckCommand() :
            AbstractCommand<transport::Command>() {
                initialize( getFrame() );
        }

        AckCommand( StompFrame* frame ) :
            AbstractCommand<transport::Command>( frame ) {
                validate( getFrame() );
        }

        virtual ~AckCommand() {}

        /**
         * Get the Message Id of this Command
         * @return the Id of the Message
         */
        virtual std::string getMessageId() const{
            return getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_MESSAGEID ), "" );
        }

        /**
         * Set the Message Id that this Ack is associated with
         * @param messageId the Message Id
         */
        virtual void setMessageId( const std::string& messageId ){
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_MESSAGEID ),
                messageId );
        }

        /**
         * Clone the StompCommand and return the new copy.
         * @returns new copy of this command caller owns it.
         */
        virtual StompCommand* cloneStompCommand() const {
            return new AckCommand( getFrame().clone() );
        }

    protected:

        /**
         * Inheritors are required to override this method to init the
         * frame with data appropriate for the command type.
         * @param frame Frame to init
         */
        virtual void initialize( StompFrame& frame ) {
            frame.setCommand( CommandConstants::toString(
                CommandConstants::ACK ) );
        }

        /**
         * Inheritors are required to override this method to validate
         * the passed stomp frame before it is marshalled or unmarshaled
         * @param frame Frame to validate
         * @returns true if frame is valid
         */
        virtual bool validate( const StompFrame& frame ) const {

            // Make sure the message is an ACK message.
            bool isAck = frame.getCommand() ==
                CommandConstants::toString( CommandConstants::ACK );

            // Make sure it has a message ID header.
            bool hasMessageId = frame.getProperties().hasProperty(
                   CommandConstants::toString(
                       CommandConstants::HEADER_MESSAGEID ) );

            if( isAck && hasMessageId ) {
                return true;
            }

            return false;
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNCETOR_STOMP_COMMANDS_ACKCOMMAND_H_*/
