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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_COMMITCOMMAND_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_COMMITCOMMAND_H_

#include <activemq/util/Config.h>
#include <activemq/connector/stomp/commands/AbstractCommand.h>
#include <activemq/connector/stomp/commands/CommandConstants.h>
#include <activemq/transport/Command.h>
#include <string>

namespace activemq{
namespace connector{
namespace stomp{
namespace commands{

    /**
     * Commits a Transaction.
     */
    class AMQCPP_API CommitCommand  : public AbstractCommand< transport::Command > {
    public:

        CommitCommand() :
            AbstractCommand<transport::Command>() {
                initialize( getFrame() );
        }

        CommitCommand( StompFrame* frame ) :
            AbstractCommand<transport::Command>( frame ) {
                validate( getFrame() );
        }

        virtual ~CommitCommand() {}

        /**
         * Clone the StompCommand and return the new copy.
         * @returns new copy of this command caller owns it.
         */
        virtual StompCommand* cloneStompCommand() const {
            return new CommitCommand( getFrame().clone() );
        }

    protected:

        /**
         * Inheritors are required to override this method to init the
         * frame with data appropriate for the command type.
         * @param frame Frame to init
         */
        virtual void initialize( StompFrame& frame ) {
            frame.setCommand( CommandConstants::toString(
                CommandConstants::COMMIT ) );
        }

        /**
         * Inheritors are required to override this method to validate
         * the passed stomp frame before it is marshalled or unmarshaled
         * @param frame Frame to validate
         * @returns true if frame is valid
         */
        virtual bool validate( const StompFrame& frame ) const {
            if((frame.getCommand() ==
                CommandConstants::toString( CommandConstants::COMMIT )) &&
               (frame.getProperties().hasProperty(
                    CommandConstants::toString(
                        CommandConstants::HEADER_TRANSACTIONID ) ) ) ) {

                return true;
            }

            return false;
        }

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_COMMANDS_COMMITCOMMAND_H_*/
