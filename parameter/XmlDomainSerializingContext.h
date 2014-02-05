/*
 * Copyright (c) 2011-2014, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include "XmlElementSerializingContext.h"

class CParameterBlackboard;
class CSelectionCriteriaDefinition;

class CXmlDomainSerializingContext : public CXmlElementSerializingContext
{
public:
    CXmlDomainSerializingContext(string& strError, bool bWithSettings);

    // Settings to be serialized or not
    bool withSettings() const;

    // Value interpretation as Real or Raw
    void setValueSpaceRaw(bool bIsRaw);
    bool valueSpaceIsRaw() const;

    // Output Raw Format for user get value interpretation
    void setOutputRawFormat(bool bIsHex);
    bool outputRawFormatIsHex();

    // Criteria defintion
    void setSelectionCriteriaDefinition(const CSelectionCriteriaDefinition* pSelectionCriteriaDefinition);
    const CSelectionCriteriaDefinition* getSelectionCriteriaDefinition() const;

    // Auto validation of configurations
    void setAutoValidationRequired(bool bAutoValidationRequired);
    bool autoValidationRequired() const;
private:
    // Indicate wheter or not to import settings
    bool _bWithSettings;
    // Value Space
    bool _bValueSpaceIsRaw;
    // Output Raw Format
    bool _bOutputRawFormatIsHex;
    // Criteria defintion
    const CSelectionCriteriaDefinition* _pSelectionCriteriaDefinition;
    // Auto validation of configurations
    bool _bAutoValidationRequired;
};
