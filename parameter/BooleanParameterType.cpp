/* 
 * INTEL CONFIDENTIAL
 * Copyright © 2011 Intel 
 * Corporation All Rights Reserved.
 * 
 * The source code contained or described herein and all documents related to
 * the source code ("Material") are owned by Intel Corporation or its suppliers
 * or licensors. Title to the Material remains with Intel Corporation or its
 * suppliers and licensors. The Material contains trade secrets and proprietary
 * and confidential information of Intel or its suppliers and licensors. The
 * Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel’s prior express written permission.
 * 
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Intel in writing.
 * 
 * CREATED: 2011-06-01
 * UPDATED: 2011-07-27
 */
#include "BooleanParameterType.h"
#include "ParameterAccessContext.h"

#define base CParameterType

CBooleanParameterType::CBooleanParameterType(const string& strName) : base(strName)
{
    setSize(1);
}

CBooleanParameterType::~CBooleanParameterType()
{
}

string CBooleanParameterType::getKind() const
{
    return "BooleanParameter";
}

// Tuning interface
bool CBooleanParameterType::toBlackboard(const string& strValue, uint32_t& uiValue, CParameterAccessContext& parameterAccessContext) const
{
    if (strValue == "1" || strValue == "0x1") {

        uiValue = true;
    } else if (strValue == "0" || strValue == "0x0") {

        uiValue = false;
    } else {
        parameterAccessContext.setError(strValue + " value not part of numerical space {");

        // Hexa
        bool bValueProvidedAsHexa = !strValue.compare(0, 2, "0x");

        if (bValueProvidedAsHexa) {

            parameterAccessContext.appendToError("0x0, 0x1");
        } else {

            parameterAccessContext.appendToError("0, 1");
        }
        parameterAccessContext.appendToError("} for " + getKind());

        return false;
    }

    return true;
}

bool CBooleanParameterType::fromBlackboard(string& strValue, const uint32_t& uiValue, CParameterAccessContext& parameterAccessContext) const
{
    strValue = uiValue ? "1" : "0";

    if (parameterAccessContext.valueSpaceIsRaw() && parameterAccessContext.outputRawFormatIsHex()) {

        strValue = "0x" + strValue;
    }

    return true;
}

// Value access
bool CBooleanParameterType::toBlackboard(bool bUserValue, uint32_t& uiValue, CParameterAccessContext& parameterAccessContext) const
{
    (void)parameterAccessContext;

    uiValue = bUserValue;

    return true;
}

bool CBooleanParameterType::fromBlackboard(bool& bUserValue, uint32_t uiValue, CParameterAccessContext& parameterAccessContext) const
{
    (void)parameterAccessContext;

    bUserValue = uiValue != 0;

    return true;
}

// Integer
bool CBooleanParameterType::toBlackboard(uint32_t uiUserValue, uint32_t& uiValue, CParameterAccessContext& parameterAccessContext) const
{
    if (uiUserValue > 1) {

        parameterAccessContext.setError("Value out of range");
    }

    uiValue = uiUserValue;

    return true;
}

bool CBooleanParameterType::fromBlackboard(uint32_t& uiUserValue, uint32_t uiValue, CParameterAccessContext& parameterAccessContext) const
{
    (void)parameterAccessContext;

    uiUserValue = uiValue != 0;

    return true;
}
