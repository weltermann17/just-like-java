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
#ifndef _DECAF_UTIL_LOGGING_SIMPLEFORMATTER_H_
#define _DECAF_UTIL_LOGGING_SIMPLEFORMATTER_H_

#include <decaf/util/logging/formatter.h>

namespace decaf{
namespace util{
namespace logging{

    /**
     * Print a brief summary of the LogRecord in a human readable format.
     * The summary will typically be 1 or 2 lines.
     */
    class DECAF_API SimpleFormatter : public Formatter {
    public:

        SimpleFormatter() {}
        virtual ~SimpleFormatter() {}

        /**
         * Format the given log record and return the formatted string.
         * @param record The Log Record to Format
         */
        virtual std::string format( const LogRecord& record ) const {
            return "";
        }

        /**
         * Format the message string from a log record.
         * @param record The Log Record to Format
         */
         virtual std::string formatMessage( const LogRecord& record ) const{
            return record.getMessage();
        }

        /**
         * Return the header string for a set of formatted records.  In the
         * default implementation this method should return empty string
         * @param handler the target handler, can be null
         * @return empty string
         */
        virtual std::string getHead( const Handler* handler ) {
            return "";
        }

        /**
         * Return the tail string for a set of formatted records.  In the
         * default implementation this method should return empty string
         * @param handler the target handler, can be null
         * @return empty string
         */
        virtual std::string getTail( const Handler* handler ) {
            return "";
        }

    };

}}}

#endif /*_DECAF_UTIL_LOGGING_SIMPLEFORMATTER_H_*/
