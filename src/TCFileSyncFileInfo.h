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
//  $Id: TCFileSyncFileInfo.h,v 5121974dd9eb 2012/09/17 19:34:55 thomas $
//*******************************************************************************
#ifndef _TC_FILE_SYNC_FILE_INFO_H_
#define _TC_FILE_SYNC_FILE_INFO_H_

#include "TCFile.h"

#include <vector>
#include <set>
#include <map>
#include <string>

namespace tc
{
   namespace file_sync
   {
      typedef std::vector<std::string>        FileNames;
      typedef std::set<std::string>           FileNamesSorted;
      
      class FileInfo
      {
      public:
         FileInfo()
         {
         }

         FileInfo(const file::FileInfo& info)
            :m_dir()
            ,m_info(info)
            ,m_hash()
         {
         }

         const std::string& GetName() const {return m_info.name;}
         std::string GetFullName() const;
         void SetName(const std::string& name) {m_info.name = name;}
         void SetDir(const std::string& dir) {m_dir = dir;}
         bool IsDirectory() const {return m_info.is_directory;}
         uint64_t GetLastModified() const {return m_info.last_modified;}
         uint64_t GetFileSize() const {return m_info.file_size;}
         const std::string& GetHashValue() const;

         bool CalculateHash() const;
      private:
         std::string m_dir;
         file::FileInfo m_info;
         mutable std::string m_hash;
      };

      typedef std::map<std::string, FileInfo> FileInfos;
   }
}

#endif // _TC_FILE_SYNC_FILE_INFO_H_

