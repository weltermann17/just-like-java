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

#ifndef _CMS_TEMPORARYTOPIC_H_
#define _CMS_TEMPORARYTOPIC_H_

#include <cms/Config.h>
#include <cms/Destination.h>
#include <cms/CMSException.h>

namespace cms{

    /**
     * An interface encapsulating a provider-specific topic name.
     */
    class CMS_API TemporaryTopic : public Destination
    {
    public:

        virtual ~TemporaryTopic() {}

        /**
         * Gets the name of this topic.
         * 
         * @return The topic name.
         */
        virtual std::string getTopicName()
            const throw( CMSException ) = 0;

        /**
         * Destroy's the Temp Destination at the Broker
         * 
         * @throws CMSException
         */
        virtual void destroy() throw ( CMSException ) = 0;

   };

}

#endif /*_CMS_TEMPORARYTOPIC_H_*/
