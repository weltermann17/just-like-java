/*
 * Copyright (c) 2001-2008
 *     DecisionSoft Limited. All rights reserved.
 * Copyright (c) 2004-2008
 *     Oracle. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * $Id: FunctionNamespaceUri.hpp 659 2008-10-06 00:11:22Z jpcs $
 */

/*

   Namespace-uri function

*/

#ifndef _FUNCTIONNAMESPACEURI_HPP
#define _FUNCTIONNAMESPACEURI_HPP

#include <xqilla/framework/XQillaExport.hpp>

#include <xqilla/ast/XQFunction.hpp>

/** Namespace-uri function */
class XQILLA_API FunctionNamespaceUri : public XQFunction
{
public:
  static const XMLCh name[];
  static const unsigned int minArgs;
  static const unsigned int maxArgs;

  FunctionNamespaceUri(const VectorOfASTNodes &args, XPath2MemoryManager* memMgr);
  
  /**
   * Called during static analysis to determine if statically correct.
   * Performs constant folding if the function has an argument, and it
   * is constant.
   */
  virtual ASTNode* staticResolution(StaticContext *context);
  virtual ASTNode *staticTypingImpl(StaticContext *context);

  /** Returns the namespace of the node. Takes an optional node,
      and returns an empty string if the node has no name. Uses the current context
      if a node is not specified. */
  Sequence createSequence(DynamicContext* context, int flags=0) const;

};

#endif // _FUNCTIONNAMESPACEURI_HPP


