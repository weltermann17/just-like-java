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

#ifndef _ACTIVEMQ_TRANSPORT_TRANSPORTFACTORYMAPREGISTRAR_H_
#define _ACTIVEMQ_TRANSPORT_TRANSPORTFACTORYMAPREGISTRAR_H_

#include <activemq/transport/TransportFactoryMap.h>
#include <activemq/util/Config.h>

namespace activemq{
namespace transport{

    /**
     * Registers the passed in factory into the factory map, this class
     * can manage the lifetime of the registered factory (default behaviour).
     */
    class AMQCPP_API TransportFactoryMapRegistrar {
    public:

        /**
         * Constructor for this class
         * @param name of the factory to register
         * @param factory responsible for creating the transport
         * @param manageLifetime boolean indicating if this object manages the
         *        lifetime of the factory that is being registered.
         */
        TransportFactoryMapRegistrar( const std::string& name,
                                      TransportFactory* factory,
                                      bool manageLifetime = true ) {

            // Register it in the map.
            TransportFactoryMap::getInstance().
                registerTransportFactory( name, factory );

            // Store for later deletion
            this->factory = factory;
            this->manageLifetime = manageLifetime;
            this->name = name;
        }

        virtual ~TransportFactoryMapRegistrar() {

            try {

                // UnRegister it in the map.
                TransportFactoryMap::getInstance().
                    unregisterTransportFactory( name );

                if( manageLifetime ) {
                    delete factory;
                }
            } catch(...) {}
        }

        /**
         * Return a reference to the factory object that is contained in this
         * registrar.
         * @return TransportFactory reference
         */
        virtual TransportFactory& getFactory() {
            return *factory;
        }

    private:

        std::string name;
        TransportFactory* factory;
        bool manageLifetime;

    };

}}

#endif /*_ACTIVEMQ_TRANSPORT_TRANSPORTFACTORYMAPREGISTRAR_H_*/
