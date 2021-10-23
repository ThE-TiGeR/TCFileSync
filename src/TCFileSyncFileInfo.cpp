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
// Copyright (C) 2003 - 2021 Thomas Goessler. All Rights Reserved. 
//*******************************************************************************

#include "TCFileSyncFileInfo.h"

#include "TCFactory.h"
#include "TCFileName.h"
#include "TCMathFactory.h"

namespace tc::file_sync
{
   void FileInfo::CalculateHash() const
   {
      MemoryMappedFilePtr file = factory::CreateMemoryMappedFile(GetFullName(), true);
      if (!file)
      {
         m_hash.clear();
         return;
      }

      math::ChecksumPtr checksum = math::factory::CreateMD5Checksum();
      checksum->Append(file->GetReadOnlyData(), file->GetSize());

      m_hash = checksum->GetHashString();
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

