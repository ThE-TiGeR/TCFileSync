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
#pragma once

#include "TCWFile.h"

#include <utility>
#include <vector>
#include <set>
#include <map>
#include <string>

namespace tc::file_sync
{
   typedef std::vector<std::wstring>        FileNames;
   typedef std::set<std::wstring>           FileNamesSorted;
      
   class FileInfo final
   {
   public:
      FileInfo() = default;

      FileInfo(wfile::FileInfo info)
         :m_info(std::move(info))
      {
      }

      const std::wstring& GetName() const {return m_info.name;}
      std::wstring GetFullName() const;
      void SetName(const std::wstring& name) {m_info.name = name;}
      void SetDir(const std::wstring& dir) {m_dir = dir;}
      bool IsDirectory() const {return m_info.is_directory;}
      uint64_t GetLastModified() const {return m_info.last_modified;}
      uint64_t GetFileSize() const {return m_info.file_size;}
      const std::string& GetHashValue() const;

      void CalculateHash() const;
   private:
      std::wstring m_dir;
      wfile::FileInfo m_info;
      mutable std::string m_hash;
   };

   typedef std::map<std::wstring, FileInfo> FileInfos;
}


