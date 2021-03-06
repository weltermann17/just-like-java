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

#ifndef _ACTIVEMQ_CONNECTOR_STOMP_MARSHAL_MARSHALABLE_H_
#define _ACTIVEMQ_CONNECTOR_STOMP_MARSHAL_MARSHALABLE_H_

#include <activemq/util/Config.h>
#include <activemq/connector/stomp/StompFrame.h>
#include <activemq/connector/stomp/marshal/MarshalException.h>

namespace activemq{
namespace connector{
namespace stomp{
namespace marshal{

    class AMQCPP_API Marshalable {
    public:

        virtual ~Marshalable(void) {}

        /**
         * Marshals the command to a stomp frame.
         * @returns the stomp frame representation of this
         * command.
         * @throws MarshalException if the command is not
         * in a state that can be marshalled.
         */
        virtual const StompFrame& marshal(void)
            throw ( marshal::MarshalException ) = 0;

    };

}}}}

#endif /*_ACTIVEMQ_CONNECTOR_STOMP_MARSHAL_MARSHALABLE_H_*/
