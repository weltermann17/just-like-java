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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_UNSUBSCRIBECOMMAND_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_UNSUBSCRIBECOMMAND_H_

#include <activemq/connector/stomp/commands/AbstractCommand.h>
#include <activemq/connector/stomp/commands/CommandConstants.h>
#include <activemq/transport/Command.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace connector{
namespace stomp{
namespace commands{

    /**
     * Command sent to the broker to unsubscribe to a
     * topic or queue.
     */
    class AMQCPP_API UnsubscribeCommand : public AbstractCommand< transport::Command > {
    public:

        UnsubscribeCommand() :
            AbstractCommand< transport::Command >() {
                initialize( getFrame() );
        }

        UnsubscribeCommand( StompFrame* frame ) :
            AbstractCommand< transport::Command >( frame ) {
                validate( getFrame() );
        }

        virtual ~UnsubscribeCommand(void) {}

        /**
         * Clone the StompCommand and return the new copy.
         * @returns new copy of this command caller owns it.
         */
        virtual StompCommand* cloneStompCommand() const {
            return new UnsubscribeCommand( getFrame().clone() );
        }

        /**
         * Get the destination
         * @returns the Destination as a string
         */
        virtual std::string getDestination() const{
            return getPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_DESTINATION ), "" );
        }

        /**
         * Set the destination
         * @param destination the destiantion as a String
         */
        virtual void setDestination( const std::string& destination ){
            setPropertyValue(
                CommandConstants::toString(
                    CommandConstants::HEADER_DESTINATION ),
                destination );
        }

    protected:

        /**
         * Inheritors are required to override this method to init the
         * frame with data appropriate for the command type.
         * @param frame Frame to init
         */
        virtual void initialize( StompFrame& frame ) {
            frame.setCommand( CommandConstants::toString(
                CommandConstants::UNSUBSCRIBE ) );
        }

        /**
         * Inheritors are required to override this method to validate
         * the passed stomp frame before it is marshalled or unmarshaled
         * @param frame Frame to validate
         * @returns true if frame is valid
         */
        virtual bool validate( const StompFrame& frame ) const {
            if((frame.getCommand() ==
                CommandConstants::toString( CommandConstants::UNSUBSCRIBE )) &&
               (frame.getProperties().hasProperty(
                    CommandConstants::toString(
                        CommandConstants::HEADER_DESTINATION ) ) ) ) {
                return true;
            }

            return false;
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_UNSUBSCRIBECOMMAND_H_*/
