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
#ifndef _DECAF_IO_WRITER_H
#define _DECAF_IO_WRITER_H

#include <string>
#include <decaf/io/IOException.h>
#include <decaf/io/OutputStream.h>
#include <decaf/lang/exceptions/NullPointerException.h>

namespace decaf{
namespace io{

    /*
     * Writer interface for an object that wraps around an output
     * stream
     */
    class DECAF_API Writer {
    public:

        virtual ~Writer(){};

        /**
         * Sets the target output stream.
         * @param Outputstream to use
         */
        virtual void setOutputStream( OutputStream* os ) = 0;

        /**
         * Gets the target output stream.
         * @returns the output stream currently being used
         */
        virtual OutputStream* getOutputStream() = 0;

        /**
         * Writes a byte array to the output stream.
         * @param buffer a byte array
         * @param count the number of bytes in the array to write.
         * @throws IOException thrown if an error occurs.
         */
        virtual void write( const unsigned char* buffer,
                            std::size_t count )
            throw( IOException, lang::exceptions::NullPointerException ) = 0;

         /**
          * Writes a byte to the output stream.
          * @param v The value to be written.
          * @throws IOException thrown if an error occurs.
          */
         virtual void writeByte( unsigned char v ) throw( IOException ) = 0;
    };

}}

#endif /*_DECAF_IO_WRITER_H*/
