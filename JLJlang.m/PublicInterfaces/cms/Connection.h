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

#ifndef _CMS_CONNECTION_H_
#define _CMS_CONNECTION_H_

#include <cms/Config.h>
#include <cms/Startable.h>
#include <cms/Stoppable.h>
#include <cms/Closeable.h>
#include <cms/Session.h>

namespace cms
{
    class ExceptionListener;
   
    /**
     * The client's connection to its provider.
     */
    class CMS_API Connection :
        public Startable,
        public Stoppable,
        public Closeable
    {
    public:

        virtual ~Connection() {}

        /**
         * Closes this connection as well as any Sessions 
         * created from it (and those Sessions' consumers and
         * producers).
         * 
         * @throws CMSException
         */
        virtual void close() throw( CMSException ) = 0;

		/**
         * Creates an AUTO_ACKNOWLEDGE Session.
         * 
         * @throws CMSException
         */
        virtual Session* createSession() throw ( CMSException ) = 0;
				
        /**
         * Creates a new Session to work for this Connection using the
         * specified acknowledgment mode
         * 
         * @param ackMode
         *      the Acknowledgement Mode to use.
         * @throws CMSException
         */
        virtual Session* createSession( Session::AcknowledgeMode ackMode ) 
            throw ( CMSException ) = 0;

        /**
         * Get the Client Id for this session
         * 
         * @return Client Id String
         */
        virtual std::string getClientID() const = 0;      

        /**
         * Gets the registered Exception Listener for this connection
         * 
         * @return pointer to an exception listnener or NULL
         */
        virtual ExceptionListener* getExceptionListener() const = 0;

        /**
         * Sets the registed Exception Listener for this connection
         * 
         * @param listener
         *      pointer to and <code>ExceptionListener</code>
         */
        virtual void setExceptionListener( ExceptionListener* listener ) = 0;

    };

}

#endif /*_CMS_CONNECTION_H_*/
