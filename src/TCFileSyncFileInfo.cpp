//*******************************************************************************
//
// *******   ***   ***               *
//    *     *     *                  *
//    *    *      *                *****
//    *    *       ***  *   *   **   *    **    ***
//    *    *          *  * *   *     *   ****  * * *
//    *     *         *   *      *   * * *     * * *
//    *      ***   ***    *     **   **   **   *   *
//                        *
//*******************************************************************************
// see http://sourceforge.net/projects/tcsystem/ for details.
// Copyright (C) 2003 - 2018 Thomas Goessler. All Rights Reserved. 
//*******************************************************************************
//
// TCSystem is the legal property of its developers.
// Please refer to the COPYRIGHT file distributed with this source distribution.
// 
// This library is free software; you can redistribute it and/or             
// modify it under the terms of the GNU Lesser General Public                
// License as published by the Free Software Foundation; either              
// version 2.1 of the License, or (at your option) any later version.        
//                                                                           
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
//                                                                           
// You should have received a copy of the GNU Lesser General Public          
// License along with this library; if not, write to the Free Software       
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
//*******************************************************************************
//  $Id: TCFileSyncFileInfo.cpp,v 296dd690c7ca 2012/04/22 16:55:42 thomas $
//*******************************************************************************
#include "TCFileSyncFileInfo.h"

#include "TCFactory.h"
#include "TCFileName.h"
#include "TCUtil.h"
#include "TCMathFactory.h"

#include "TCNewEnable.h"

namespace tc
{
    namespace file_sync
    {
        bool FileInfo::CalculateHash() const
        {
            MemoryMappedFilePtr file = factory::CreateMemoryMappedFile(GetFullName(), true);
            if (!file)
            {
                return false;
            }

            math::ChecksumPtr checksum = math::factory::CreateMD5Checksum();
            checksum->Append(file->GetReadOnlyData(), file->GetSize());

            m_hash = checksum->GetHashString();

            return true;
        }

        const std::string& FileInfo::GetHashValue() const
        {
            if (m_hash.empty())
            {
                CalculateHash();
            }
            return m_hash;
        }

        std::string FileInfo::GetFullName() const
        {
            return file_name::AddFileNameAndPath(m_info.name, m_dir);
        }
    }
}

