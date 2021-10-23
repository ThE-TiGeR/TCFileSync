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

#include "TCFileSyncActions.h"
#include "TCFileSyncFileFinder.h"
#include "TCFileSyncSettings.h"
#include "TCFileSyncStatusDisplayer.h"

namespace tc::file_sync
{
   class ActionGenerator final
   {
   public:
      ActionGenerator(const Settings& settings, StatusDisplayerPtr status_displayer);

      bool CreateActions();
      [[nodiscard]] const Actions& GetActions() const {return m_actions;}

   private:
      FileInfos GetDifference(const FileInfos& files1, const FileInfos& files2);
      FileInfos GetModifiedFiles(const FileInfos& files1, const FileInfos& files2);

      void CreateMissingFileActions(const FileInfos& missing_files);
      void CreateModifiedFileActions(const FileInfos& modified_files);
      void CreateDeletedFileActions(const FileInfos& deleted_files);

      uint32_t CreateBackupActionsForFile(const std::wstring& source_file_name);

   private:
      FileFinder m_file_finder;
      Settings m_settings;
      Actions m_actions;
      StatusDisplayerPtr m_status_displayer;
   };
}
