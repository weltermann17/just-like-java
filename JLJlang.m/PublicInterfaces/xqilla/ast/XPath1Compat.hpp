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
 * $Id: XPath1Compat.hpp 659 2008-10-06 00:11:22Z jpcs $
 */

#ifndef _XPATH1COMPAT_HPP
#define _XPATH1COMPAT_HPP

#include <xqilla/framework/XQillaExport.hpp>
#include <xqilla/ast/ASTNodeImpl.hpp>

class SequenceType;

class XQILLA_API XPath1CompatConvertFunctionArg : public ASTNodeImpl
{
public:
  XPath1CompatConvertFunctionArg(ASTNode* expr, SequenceType *seqType, XPath2MemoryManager* memMgr);

  virtual Result createResult(DynamicContext* context, int flags=0) const;
  virtual ASTNode* staticResolution(StaticContext *context);
  virtual ASTNode *staticTypingImpl(StaticContext *context);

  ASTNode *getExpression() const { return expr_; }
  void setExpression(ASTNode *expr) { expr_ = expr; }
  SequenceType *getSequenceType() const { return seqType_; }

protected:
  ASTNode* expr_;
  SequenceType *seqType_;
};

class XPath1CompatConvertFunctionArgResult : public ResultImpl
{
public:
  XPath1CompatConvertFunctionArgResult(const LocationInfo *location, const Result &parent, const SequenceType *seqType);

  Item::Ptr next(DynamicContext *context);
  std::string asString(DynamicContext *context, int indent) const;
private:
  const SequenceType *seqType_;
  Result parent_;
  bool oneDone_;
};

#endif
