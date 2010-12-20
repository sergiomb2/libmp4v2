/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is MPEG4IP.
 *
 * The Initial Developer of the Original Code is Cisco Systems Inc.
 * Portions created by Cisco Systems Inc. are
 * Copyright (C) Cisco Systems Inc. 2001.  All Rights Reserved.
 *
 * Contributor(s):
 *      Dave Mackie     dmackie@cisco.com
 */

#include "src/impl.h"

namespace mp4v2 {
namespace impl {

///////////////////////////////////////////////////////////////////////////////

MP4Descriptor::MP4Descriptor(uint8_t tag) {
    m_tag = tag;
    m_pParentAtom = NULL;
    m_start = 0;
    m_size = 0;
    m_readMutatePoint = 0;
}

MP4Descriptor::~MP4Descriptor()
{
    for (uint32_t i = 0; i < m_pProperties.Size(); i++) {
        delete m_pProperties[i];
    }
}

void MP4Descriptor::AddProperty(MP4Property* pProperty)
{
    ASSERT(pProperty);
    m_pProperties.Add(pProperty);
    pProperty->SetParentAtom(m_pParentAtom);
}

bool MP4Descriptor::FindContainedProperty(const char *name,
        MP4Property** ppProperty, uint32_t* pIndex)
{
    uint32_t numProperties = m_pProperties.Size();

    for (uint32_t i = 0; i < numProperties; i++) {
        if (m_pProperties[i]->FindProperty(name, ppProperty, pIndex)) {
            return true;
        }
    }
    return false;
}

void MP4Descriptor::Generate()
{
    // generate properties
    for (uint32_t i = 0; i < m_pProperties.Size(); i++) {
        m_pProperties[i]->Generate();
    }
}

void MP4Descriptor::Read(MP4File* pFile)
{
    ReadHeader(pFile);

    ReadProperties(pFile, 0, m_readMutatePoint);

    Mutate();

    ReadProperties(pFile, m_readMutatePoint);

    // flush any leftover read bits
    pFile->FlushReadBits();
}

void MP4Descriptor::ReadHeader(MP4File* pFile)
{
    ASSERT(pFile);
    log.verbose1f("ReadDescriptor: pos = 0x%" PRIx64, pFile->GetPosition());

    // read tag and length
    uint8_t tag = pFile->ReadUInt8();
    if (m_tag) {
        ASSERT(tag == m_tag);
    } else {
        m_tag = tag;
    }
    m_size = pFile->ReadMpegLength();
    m_start = pFile->GetPosition();

    log.verbose1f("ReadDescriptor: tag 0x%02x data size %u (0x%x)",
                  m_tag, m_size, m_size);
}

void MP4Descriptor::ReadProperties(MP4File* pFile,
                                   uint32_t propStartIndex, uint32_t propCount)
{
    uint32_t numProperties = min(propCount,
                                 m_pProperties.Size() - propStartIndex);

    for (uint32_t i = propStartIndex;
            i < propStartIndex + numProperties; i++) {

        MP4Property* pProperty = m_pProperties[i];

        int32_t remaining = m_size - (pFile->GetPosition() - m_start);

        if (pProperty->GetType() == DescriptorProperty) {
            if (remaining > 0) {
                // place a limit on how far this sub-descriptor looks
                ((MP4DescriptorProperty*)pProperty)->SetSizeLimit(remaining);
                pProperty->Read(pFile);
            } // else do nothing, empty descriptor
        } else {
            // non-descriptor property
            if (remaining >= 0) {
                pProperty->Read(pFile);

                MP4LogLevel thisVerbosity =
                    (pProperty->GetType() == TableProperty) ?
                    MP4_LOG_VERBOSE2 : MP4_LOG_VERBOSE1;

                if (log.verbosity >= thisVerbosity) {
                    // log.printf(thisVerbosity,"Read: ");
                    pProperty->Dump(0, true);
                }
            } else {
                log.errorf("Overran descriptor, tag %u data size %u property %u",
                           m_tag, m_size, i);
                throw new Exception("overran descriptor",__FILE__, __LINE__, __FUNCTION__);
            }
        }
    }
}

void MP4Descriptor::Write(MP4File* pFile)
{
    // call virtual function to adapt properties before writing
    Mutate();

    uint32_t numProperties = m_pProperties.Size();

    if (numProperties == 0) {
        WARNING(numProperties == 0);
        return;
    }

    // write tag and length placeholder
    pFile->WriteUInt8(m_tag);
    uint64_t lengthPos = pFile->GetPosition();
    pFile->WriteMpegLength(0);
    uint64_t startPos = pFile->GetPosition();

    for (uint32_t i = 0; i < numProperties; i++) {
        m_pProperties[i]->Write(pFile);
    }

    // align with byte boundary (rarely necessary)
    pFile->PadWriteBits();

    // go back and write correct length
    uint64_t endPos = pFile->GetPosition();
    pFile->SetPosition(lengthPos);
    pFile->WriteMpegLength(endPos - startPos);
    pFile->SetPosition(endPos);
}

void MP4Descriptor::WriteToMemory(MP4File* pFile,
                                  uint8_t** ppBytes, uint64_t* pNumBytes)
{
    // use memory buffer to save descriptor in memory
    // instead of going directly to disk

    pFile->EnableMemoryBuffer();

    Write(pFile);

    pFile->DisableMemoryBuffer(ppBytes, pNumBytes);
}

void MP4Descriptor::Dump(uint8_t indent, bool dumpImplicits)
{
    // call virtual function to adapt properties before dumping
    Mutate();

    uint32_t numProperties = m_pProperties.Size();

    if (numProperties == 0) {
        WARNING(numProperties == 0);
        return;
    }
    for (uint32_t i = 0; i < numProperties; i++) {
        m_pProperties[i]->Dump(indent, dumpImplicits);
    }
}

uint8_t MP4Descriptor::GetDepth()
{
    if (m_pParentAtom) {
        return m_pParentAtom->GetDepth();
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

}
} // namespace mp4v2::impl
