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

#include "TCFileSyncFileInfo.h"
#include "TCFileSyncSettings.h"

namespace tc::file_sync
{
   class FileFinder final
   {
   public:
      FileFinder(Settings settings);

      bool Find();

      [[nodiscard]] const FileInfos& GetSourceFiles() const
      {
         return m_files_source;
      }

      [[nodiscard]] const FileInfos& GetDestinationFiles() const
      {
         return m_files_destination;
      }

   private:
      bool FindSourceFiles();
      bool FindDestinationFiles();
      bool FindRecursiveFiles(const std::wstring& directory, FileInfos& files);

      Settings m_settings;
      FileInfos m_files_source;
      FileInfos m_files_destination;
   };
}
