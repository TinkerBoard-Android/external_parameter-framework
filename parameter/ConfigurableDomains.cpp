/* <auto_header>
 * <FILENAME>
 * 
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
 *  AUTHOR: Patrick Benavoli (patrickx.benavoli@intel.com)
 * CREATED: 2011-06-01
 * UPDATED: 2011-07-27
 * 
 * 
 * </auto_header>
 */
#include "ConfigurableDomains.h"
#include "ConfigurableDomain.h"
#include "ConfigurableElement.h"
#include "BinaryStream.h"
#include "AutoLog.h"

#define base CBinarySerializableElement

CConfigurableDomains::CConfigurableDomains(const string& strSystemClassName) : base(strSystemClassName)
{
}

string CConfigurableDomains::getKind() const
{
    return "ConfigurableDomains";
}

bool CConfigurableDomains::childrenAreDynamic() const
{
    return true;
}

// Ensure validity on whole domains from main blackboard
void CConfigurableDomains::validate(const CParameterBlackboard* pMainBlackboard)
{
    // Delegate to domains
    uint32_t uiChild;
    uint32_t uiNbConfigurableDomains = getNbChildren();

    for (uiChild = 0; uiChild < uiNbConfigurableDomains; uiChild++) {

        CConfigurableDomain* pChildConfigurableDomain = static_cast<CConfigurableDomain*>(getChild(uiChild));

        pChildConfigurableDomain->validate(pMainBlackboard);
    }
}

// Configuration application if required
bool CConfigurableDomains::apply(CParameterBlackboard* pParameterBlackboard, bool bForce, string& strError)
{
   CAutoLog autoLog(this, "Applying configurations");

    // Syncer set
    CSyncerSet syncerSet;

    // Delegate to domains
    uint32_t uiChild;
    uint32_t uiNbConfigurableDomains = getNbChildren();

    for (uiChild = 0; uiChild < uiNbConfigurableDomains; uiChild++) {

        CConfigurableDomain* pChildConfigurableDomain = static_cast<CConfigurableDomain*>(getChild(uiChild));

        pChildConfigurableDomain->apply(pParameterBlackboard, syncerSet, bForce);
    }
    // Synchronize
    return syncerSet.sync(*pParameterBlackboard, false, strError);
}

// From IXmlSource
void CConfigurableDomains::toXml(CXmlElement& xmlElement, CXmlSerializingContext& serializingContext) const
{
    // Set attribute
    xmlElement.setAttributeString("SystemClassName", getName());

    base::toXml(xmlElement, serializingContext);
}

// Configuration/Domains handling
/// Domains
bool CConfigurableDomains::createDomain(const string& strName, string& strError)
{
    // Already exists?
    if (findChild(strName)) {

        strError = "Already existing configurable domain";

        return false;
    }

    log("Creating configurable domain \"%s\"", strName.c_str());

    // Creation/Hierarchy
    addChild(new CConfigurableDomain(strName));

    return true;
}

bool CConfigurableDomains::deleteDomain(const string& strName, string& strError)
{
    CConfigurableDomain* pConfigurableDomain = static_cast<CConfigurableDomain*>(findChild(strName));

    if (!pConfigurableDomain) {

        strError = "Configurable domain not found";

        return false;
    }

    // Check domain has no rule (prevent accidental loss of data)
    if (pConfigurableDomain->hasRules()) {

        strError = "Deletion of domain containing configurations with application rules is not supported to prevent any accitental loss of data.\nPlease consider a direct modification of the XML file.";

        return false;
    }

    log("Deleting configurable domain \"%s\"", strName.c_str());

    // Hierarchy
    removeChild(pConfigurableDomain);

    // Destroy
    delete pConfigurableDomain;

    return true;
}

bool CConfigurableDomains::renameDomain(const string& strName, const string& strNewName, string& strError)
{
    CConfigurableDomain* pConfigurableDomain = static_cast<CConfigurableDomain*>(findChild(strName));

    if (!pConfigurableDomain) {

        strError = "Configurable domain not found";

        return false;
    }

    log("Renaming configurable domain \"%s\" to \"%s\"", strName.c_str(), strNewName.c_str());

    // Rename
    return pConfigurableDomain->rename(strNewName, strError);
}

/// Configurations
bool CConfigurableDomains::listConfigurations(const string& strDomain, string& strResult) const
{
    const CElement* pConfigurableDomain = findChild(strDomain);

    if (!pConfigurableDomain) {

        strResult = "Configurable domain not found";

        return false;
    }
    // delegate
    pConfigurableDomain->listChildren(strResult);

    return true;
}

bool CConfigurableDomains::createConfiguration(const string& strDomain, const string& strConfiguration, const CParameterBlackboard* pMainBlackboard, string& strError)
{
    // Find domain
    CConfigurableDomain* pConfigurableDomain = static_cast<CConfigurableDomain*>(findChild(strDomain));

    if (!pConfigurableDomain) {

        strError = "Configurable domain " + strDomain + " not found";

        return false;
    }
    // Delegate
    return pConfigurableDomain->createConfiguration(strConfiguration, pMainBlackboard, strError);
}

bool CConfigurableDomains::deleteConfiguration(const string& strDomain, const string& strConfiguration, string& strError)
{
    // Find domain
    CConfigurableDomain* pConfigurableDomain = static_cast<CConfigurableDomain*>(findChild(strDomain));

    if (!pConfigurableDomain) {

        strError = "Configurable domain " + strDomain + " not found";

        return false;
    }
    // Delegate
    return pConfigurableDomain->deleteConfiguration(strConfiguration, strError);
}

bool CConfigurableDomains::renameConfiguration(const string& strDomain, const string& strConfigurationName, const string& strNewConfigurationName, string& strError)
{
    // Find domain
    CConfigurableDomain* pConfigurableDomain = static_cast<CConfigurableDomain*>(findChild(strDomain));

    if (!pConfigurableDomain) {

        strError = "Configurable domain " + strDomain + " not found";

        return false;
    }
    // Delegate
    return pConfigurableDomain->renameConfiguration(strConfigurationName, strNewConfigurationName, strError);
}

bool CConfigurableDomains::listDomainElements(const string& strDomain, string& strResult) const
{
    // Find domain
    const CConfigurableDomain* pConfigurableDomain = static_cast<const CConfigurableDomain*>(findChild(strDomain));

    if (!pConfigurableDomain) {

        strResult = "Configurable domain " + strDomain + " not found";

        return false;
    }
    // Delegate
    pConfigurableDomain->listAssociatedToElements(strResult);

    return true;
}

bool CConfigurableDomains::split(const string& strDomain, CConfigurableElement* pConfigurableElement, string& strError)
{
    // Find domain
    CConfigurableDomain* pConfigurableDomain = static_cast<CConfigurableDomain*>(findChild(strDomain));

    if (!pConfigurableDomain) {

        strError = "Configurable domain " + strDomain + " not found";

        return false;
    }
    // Delegate
    pConfigurableDomain->split(pConfigurableElement, strError);

    return true;
}

void CConfigurableDomains::listAssociatedElements(string& strResult) const
{
    strResult = "\n";

    set<const CConfigurableElement*> configurableElementSet;

    // Get all owned configurable elements
    gatherAllOwnedConfigurableElements(configurableElementSet);

    // Fill result
    set<const CConfigurableElement*>::const_iterator it;

    for (it = configurableElementSet.begin(); it != configurableElementSet.end(); ++it) {

        const CConfigurableElement* pConfigurableElement = *it;

        string strAssociatedDomainList;

        pConfigurableElement->listAssociatedDomains(strAssociatedDomainList, false);

        strResult += pConfigurableElement->getPath() + " [" + strAssociatedDomainList + "]\n";
    }
}

void CConfigurableDomains::listConflictingElements(string& strResult) const
{
    strResult = "\n";

    set<const CConfigurableElement*> configurableElementSet;

    // Get all owned configurable elements
    gatherAllOwnedConfigurableElements(configurableElementSet);

    // Fill result
    set<const CConfigurableElement*>::const_iterator it;

    for (it = configurableElementSet.begin(); it != configurableElementSet.end(); ++it) {

        const CConfigurableElement* pConfigurableElement = *it;

        if (pConfigurableElement->getBelongingDomainCount() > 1) {

            string strBelongingDomainList;

            pConfigurableElement->listBelongingDomains(strBelongingDomainList, false);

            strResult += pConfigurableElement->getPath() + " contained in multiple domains: " + strBelongingDomainList + "\n";
        }
    }
}

// Gather configurable elements owned by any domain
void CConfigurableDomains::gatherAllOwnedConfigurableElements(set<const CConfigurableElement*>& configurableElementSet) const
{
    // Delegate to domains
    uint32_t uiChild;
    uint32_t uiNbConfigurableDomains = getNbChildren();

    for (uiChild = 0; uiChild < uiNbConfigurableDomains; uiChild++) {

        const CConfigurableDomain* pChildConfigurableDomain = static_cast<const CConfigurableDomain*>(getChild(uiChild));

        pChildConfigurableDomain->gatherConfigurableElements(configurableElementSet);
    }
}

// Config restore
bool CConfigurableDomains::restoreConfiguration(const string& strDomain, const string& strConfiguration, CParameterBlackboard* pMainBlackboard, bool bAutoSync, string& strError)
{
    // Find domain
    CConfigurableDomain* pConfigurableDomain = static_cast<CConfigurableDomain*>(findChild(strDomain));

    if (!pConfigurableDomain) {

        strError = "Configurable domain " + strDomain + " not found";

        return false;
    }
    // Delegate
    return pConfigurableDomain->restoreConfiguration(strConfiguration, pMainBlackboard, bAutoSync, strError);
}

// Config save
bool CConfigurableDomains::saveConfiguration(const string& strDomain, const string& strConfiguration, const CParameterBlackboard* pMainBlackboard, string& strError)
{
    // Find domain
    CConfigurableDomain* pConfigurableDomain = static_cast<CConfigurableDomain*>(findChild(strDomain));

    if (!pConfigurableDomain) {

        strError = "Configurable domain " + strDomain + " not found";

        return false;
    }
    // Delegate
    return pConfigurableDomain->saveConfiguration(strConfiguration, pMainBlackboard, strError);
}

// Last applied configurations
void CConfigurableDomains::listLastAppliedConfigurations(string& strResult) const
{
    // Browse domains
    uint32_t uiChild;
    uint32_t uiNbConfigurableDomains = getNbChildren();

    for (uiChild = 0; uiChild < uiNbConfigurableDomains; uiChild++) {

        const CConfigurableDomain* pChildConfigurableDomain = static_cast<const CConfigurableDomain*>(getChild(uiChild));

        strResult += pChildConfigurableDomain->getName() + ": " + pChildConfigurableDomain->getLastAppliedConfigurationName() + "\n";
    }
}

// Configurable element - domain association
bool CConfigurableDomains::addConfigurableElementToDomain(const string& strDomain, CConfigurableElement* pConfigurableElement, const CParameterBlackboard* pMainBlackboard, string& strError)
{
    // Find domain
    CConfigurableDomain* pConfigurableDomain = static_cast<CConfigurableDomain*>(findChild(strDomain));

    if (!pConfigurableDomain) {

        strError = "Configurable domain " + strDomain + " not found";

        return false;
    }
    // Delegate
    return pConfigurableDomain->addConfigurableElement(pConfigurableElement, pMainBlackboard, strError);
}

bool CConfigurableDomains::removeConfigurableElementFromDomain(const string& strDomain, CConfigurableElement* pConfigurableElement, string& strError)
{
    // Find domain
    CConfigurableDomain* pConfigurableDomain = static_cast<CConfigurableDomain*>(findChild(strDomain));

    if (!pConfigurableDomain) {

        strError = "Configurable domain " + strDomain + " not found";

        return false;
    }
    // Delegate
    return pConfigurableDomain->removeConfigurableElement(pConfigurableElement, strError);
}

// Binary settings load/store
bool CConfigurableDomains::serializeSettings(const string& strBinarySettingsFilePath, bool bOut, uint8_t uiStructureChecksum, string& strError)
{
    // Instantiate byte stream
    CBinaryStream binarySettingsStream(strBinarySettingsFilePath, bOut, getDataSize(), uiStructureChecksum);

    // Open file
    if (!binarySettingsStream.open(strError)) {

        strError = "Unable to open binary settings file " + strBinarySettingsFilePath + ": " + strError;

        return false;
    }

    // Serialize
    binarySerialize(binarySettingsStream);

    // Close stream
    binarySettingsStream.close();

    return true;
}
