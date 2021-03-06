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

#ifndef ACTIVEMQ_TRANSPORT_TRANSPORTFILTER_H_
#define ACTIVEMQ_TRANSPORT_TRANSPORTFILTER_H_

#include <activemq/util/Config.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/CommandListener.h>
#include <activemq/transport/Command.h>
#include <activemq/transport/TransportExceptionListener.h>
#include <typeinfo>

namespace activemq{
namespace transport{

    /**
     * A filter on the transport layer.  Transport
     * filters implement the Transport interface and
     * optionally delegate calls to another Transport object.
     */
    class AMQCPP_API TransportFilter :
        public Transport,
        public CommandListener,
        public TransportExceptionListener {

    protected:

        /**
         * The transport that this filter wraps around.
         */
        Transport* next;

        /**
         * Flag to indicate whether this object controls
         * the lifetime of the next transport object.
         */
        bool own;

        /**
         * Listener to incoming commands.
         */
        CommandListener* commandlistener;

        /**
         * Listener of exceptions from this transport.
         */
        TransportExceptionListener* exceptionListener;

    protected:

        /**
         * Notify the excpetion listener
         * @param ex - the exception to send to listeners
         */
        void fire( const decaf::lang::Exception& ex ){

            if( exceptionListener != NULL ){
                try{
                    exceptionListener->onTransportException( this, ex );
                }catch( ... ){}
            }
        }

        /**
         * Notify the command listener.
         * @param command - the command to send to the listener
         */
        void fire( Command* command ){
            try{
                if( commandlistener != NULL ){
                    commandlistener->onCommand( command );
                }
            }catch( ... ){}
        }

    public:

        /**
         * Constructor.
         * @param next - the next Transport in the chain
         * @param own - true if this filter owns the next and should delete it
         */
        TransportFilter( Transport* next, const bool own = true );

        virtual ~TransportFilter();

        /**
         * Event handler for the receipt of a command.
         * @param command - the received command object.
         */
        virtual void onCommand( Command* command ){
            fire( command );
        }

        /**
         * Event handler for an exception from a command transport.
         * @param source The source of the exception
         * @param ex The exception.
         */
        virtual void onTransportException( Transport* source,
                                           const decaf::lang::Exception& ex );

        /**
         * Sends a one-way command.  Does not wait for any response from the
         * broker.
         * @param command the command to be sent.
         * @throws CommandIOException if an exception occurs during writing of
         * the command.
         * @throws UnsupportedOperationException if this method is not implemented
         * by this transport.
         */
        virtual void oneway( Command* command )
            throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException ){

            next->oneway( command );
        }

        /**
         * Not supported by this class - throws an exception.
         * @param command the command that is sent as a request
         * @throws CommandIOException
         * @throws UnsupportedOperationException.
         */
        virtual Response* request( Command* command )
            throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException ){

            return next->request( command );
        }

        /**
         * Not supported by this class - throws an exception.
         * @param command - The command that is sent as a request
         * @param timeout - The the time to wait for a response.
         * @throws CommandIOException
         * @throws UnsupportedOperationException.
         */
        virtual Response* request( Command* command, unsigned int timeout )
            throw( CommandIOException, decaf::lang::exceptions::UnsupportedOperationException ){

            return next->request( command, timeout );
        }

        /**
         * Assigns the command listener for non-response commands.
         * @param listener the listener.
         */
        virtual void setCommandListener( CommandListener* listener ){
            this->commandlistener = listener;
        }

        /**
         * Sets the command reader.
         * @param reader the object that will be used for reading command objects.
         */
        virtual void setCommandReader( CommandReader* reader ){
            next->setCommandReader( reader );
        }

        /**
         * Sets the command writer.
         * @param writer the object that will be used for writing command objects.
         */
        virtual void setCommandWriter( CommandWriter* writer ){
            next->setCommandWriter( writer );
        }

        /**
         * Sets the observer of asynchronous exceptions from this transport.
         * @param listener the listener of transport exceptions.
         */
        virtual void setTransportExceptionListener( TransportExceptionListener* listener ){
            this->exceptionListener = listener;
        }

        /**
         * Starts this transport object and creates the thread for
         * polling on the input stream for commands.  If this object
         * has been closed, throws an exception.  Before calling start,
         * the caller must set the IO streams and the reader and writer
         * objects.
         * @throws CMSException if an error occurs or if this transport
         * has already been closed.
         */
        virtual void start() throw( cms::CMSException ) {

            if( commandlistener == NULL ){
                throw exceptions::ActiveMQException( __FILE__, __LINE__,
                    "commandListener is invalid" );
            }

            if( exceptionListener == NULL ){
                throw exceptions::ActiveMQException( __FILE__, __LINE__,
                    "exceptionListener is invalid" );
            }

            // Start the delegate transport object.
            next->start();
        }

        /**
         * Stops the polling thread and closes the streams.  This can
         * be called explicitly, but is also called in the destructor. Once
         * this object has been closed, it cannot be restarted.
         * @throws CMSException if errors occur.
         */
        virtual void close() throw( cms::CMSException ){
            next->close();
        }

        /**
         * Narrows down a Chain of Transports to a specific Transport to allow a
         * higher level transport to skip intermediate Transports in certain
         * circumstances.
         *
         * @param typeId - The type_info of the Object we are searching for.
         *
         * @return the requested Object. or NULL if its not in this chain.
         */
        virtual Transport* narrow( const std::type_info& typeId ) {
            if( typeid( *this ) == typeId ) {
                return this;
            } else if( this->next != NULL ) {
                return this->next->narrow( typeId );
            }

            return NULL;
        }

    };

}}

#endif /*ACTIVEMQ_TRANSPORT_TRANSPORTFILTER_H_*/
