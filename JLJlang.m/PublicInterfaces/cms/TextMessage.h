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

#ifndef _CMS_TEXTMESSAGE_H_
#define _CMS_TEXTMESSAGE_H_

#include <cms/Config.h>
#include <cms/Message.h>
#include <cms/CMSException.h>

namespace cms{
    
    /**
     * Interface for a text message.
     */
    class CMS_API TextMessage : public Message{

    public:

        virtual ~TextMessage(){}

        /**
         * Gets the message character buffer.
         * 
         * @return The message character buffer.
         */
        virtual std::string getText() const throw( CMSException ) = 0;

        /**
         * Sets the message contents, does not take ownership of the passed
         * char*, but copies it instead.
         * 
         * @param msg 
         *      The message buffer.
         */
        virtual void setText( const char* msg ) throw( CMSException ) = 0;

        /**
         * Sets the message contents
         * 
         * @param msg 
         *      The message buffer.
         */
        virtual void setText( const std::string& msg ) throw( CMSException ) = 0;

    };
}

#endif /*_CMS_TEXTMESSAGE_H_*/
