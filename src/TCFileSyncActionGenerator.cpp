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

#include "TCFileSyncActionGenerator.h"

#include <utility>

#include "TCFile.h"
#include "TCFileName.h"
#include "TCOutput.h"
#include "TCString.h"

namespace tc::file_sync
{
   ActionGenerator::ActionGenerator(const Settings& settings, StatusDisplayerPtr status_displayer)
      :m_file_finder(settings)
       , m_settings(settings)
       , m_status_displayer(std::move(status_displayer))
   {

      m_settings.backup_folder = file_name::AddFileNameAndPath(settings.backup_folder, settings.destination);
   }

   bool ActionGenerator::CreateActions()
   {
      if (!m_file_finder.Find()) return false;

      TCINFOS("FileSync", "Finding missing destination files ...");
      FileInfos missing_files = GetDifference(m_file_finder.GetSourceFiles(), m_file_finder.GetDestinationFiles());
      TCINFOS("FileSync", "Found " << missing_files.size() << " missing destination files.");

      TCINFOS("FileSync", "Finding deleted source files ...");
      FileInfos deleted_files = GetDifference(m_file_finder.GetDestinationFiles(), m_file_finder.GetSourceFiles());
      TCINFOS("FileSync", "Found " << deleted_files.size() << " deleted source files.");

      FileInfos modified_files = GetModifiedFiles(m_file_finder.GetSourceFiles(), m_file_finder.GetDestinationFiles());

      CreateMissingFileActions(missing_files);
      CreateDeletedFileActions(deleted_files);
      CreateModifiedFileActions(modified_files);

      return true;
   }

   FileInfos ActionGenerator::GetDifference(const FileInfos& files1,
                                            const FileInfos& files2)
   {
      FileInfos diff_files;
      for (const auto & file : files1)
      {
         if (files2.find(file.second.GetName()) == files2.end())
         {
            diff_files.insert(file);
         }
      }

      return diff_files;
   }

   FileInfos ActionGenerator::GetModifiedFiles(const FileInfos& files1,
                                               const FileInfos& files2)
   {
      TCINFOS("FileSync", "Finding modified source files ...");
      if (m_status_displayer && m_settings.calc_checksum)
      {
         m_status_displayer->SetStatusText("Finding modified source files");
      }

      FileInfos modified_files;

      uint64_t total_bytes = 0;
      for (const auto & file : files1)
      {
         const FileInfo& file_info = file.second;
         total_bytes += file_info.GetFileSize();
      }

      uint64_t bytes_done = 0;
      for (const auto & file : files1)
      {
         const FileInfo& file_info = file.second;
         // directories we do not need to check
         if (file_info.IsDirectory())
         {
            continue;
         }

         FileInfos::const_iterator found_file;
         found_file = files2.find(file_info.GetName());
         if (found_file != files2.end())
         {
            if (file_info.GetLastModified() > found_file->second.GetLastModified())
            {
               modified_files[file_info.GetName()] = file_info;
            }
            else if (m_settings.calc_checksum)
            {
               if (m_status_displayer)
               {
                  m_status_displayer->SetProgress(0, bytes_done, total_bytes);
               }

               if (file_info.GetHashValue() != found_file->second.GetHashValue())
               {
                  modified_files[file_info.GetName()] = file_info;
               }
            }

            bytes_done += file_info.GetFileSize();
         }
      }

      if (m_status_displayer)
      {
         m_status_displayer->SetStatusText("");
      }

      TCINFOS("FileSync", "Found " << modified_files.size() << " modified source files.");
      return modified_files;
   }

   void ActionGenerator::CreateMissingFileActions(const FileInfos& missing_files)
   {
      for (const auto & missing_file : missing_files)
      {
         FileInfo dest_file_info = missing_file.second;
         dest_file_info.SetName(file_name::AddFileNameAndPath(missing_file.second.GetName(), m_settings.destination));

         if (!dest_file_info.IsDirectory())
         {
            FileInfo source_file_info = missing_file.second;
            source_file_info.SetName(file_name::AddFileNameAndPath(missing_file.second.GetName(), m_settings.source));
            m_actions.push_back(CreateCopyAction(source_file_info, dest_file_info));
         }
      }
   }

   uint32_t ActionGenerator::CreateBackupActionsForFile(const std::string& source_file_name)
   {
      std::string ext = file_name::GetExtension(source_file_name);
      std::string file_name = file_name::RemoveExtension(source_file_name);
      std::string path = file_name::GetPath(source_file_name);

      uint32_t backup_id = 1;
      Actions actions;
      for (uint32_t file_idx = m_settings.num_backups; file_idx > 0; file_idx--)
      {
         std::string backup_id1 = "$" + string::ToString(file_idx) + "$";
         std::string file_name1 = file_name::AddFileNameAndExtension(file_name, backup_id1);
         file_name1 = file_name::AddFileNameAndExtension(file_name1, ext);
         file_name1 = file_name::AddFileNameAndPath(file_name1, m_settings.backup_folder);
         // if file does not exist nothing needs to be done
         if (!file::Exists(file_name1))
         {
            continue;
         }

         // setup backup id
         // if we ar at not maximum backup id we do not need to move old backups
         // so we break
         if (backup_id == 1)
         {
            if (file_idx != m_settings.num_backups)
            {
               backup_id = file_idx + 1;
               break;
            }
            backup_id = file_idx;
         }

         if (file_idx == 1) continue;

         std::string backup_id2 = "$" + string::ToString(file_idx - 1) + "$";
         std::string file_name2 = file_name::AddFileNameAndExtension(file_name, backup_id2);
         file_name2 = file_name::AddFileNameAndExtension(file_name2, ext);
         file_name2 = file_name::AddFileNameAndPath(file_name2, m_settings.backup_folder);

         FileInfo file_info1;
         file_info1.SetName(file_name1);
         FileInfo file_info2;
         file_info2.SetName(file_name2);
         actions.push_back(CreateMoveAction(file_info1, file_info2));
      }

      m_actions.insert(m_actions.end(), actions.rbegin(), actions.rend());

      return backup_id;
   }

   void ActionGenerator::CreateModifiedFileActions(const FileInfos& modified_files)
   {
      for (const auto & modified_file : modified_files)
      {
         FileInfo source_file_info = modified_file.second;
         source_file_info.SetName(file_name::AddFileNameAndPath(modified_file.second.GetName(), m_settings.source));

         FileInfo dest_file_info = modified_file.second;
         dest_file_info.SetName(file_name::AddFileNameAndPath(modified_file.second.GetName(), m_settings.destination));

         if (m_settings.num_backups > 0)
         {
            uint32_t backup_id = CreateBackupActionsForFile(modified_file.second.GetName());

            std::string ext = file_name::GetExtension(modified_file.second.GetName());
            std::string file_name = file_name::RemoveExtension(modified_file.second.GetName());
            std::string backup_name = "$" + string::ToString(backup_id) + "$";
            file_name = file_name::AddFileNameAndExtension(file_name, backup_name);
            file_name = file_name::AddFileNameAndExtension(file_name, ext);

            FileInfo move_file_info = modified_file.second;
            move_file_info.SetName(file_name::AddFileNameAndPath(file_name, m_settings.backup_folder));

            m_actions.push_back(CreateMoveAction(dest_file_info, move_file_info));
         }

         m_actions.push_back(CreateCopyAction(source_file_info, dest_file_info));
      }
   }

   void ActionGenerator::CreateDeletedFileActions(const FileInfos& deleted_files)
   {
      FileInfos::const_reverse_iterator file_info_it;
      for (file_info_it = deleted_files.rbegin(); file_info_it != deleted_files.rend(); file_info_it++)
      {
         FileInfo source_file_info = file_info_it->second;
         source_file_info.SetName(file_name::AddFileNameAndPath(file_info_it->second.GetName(), m_settings.destination));

         if (file_info_it->second.IsDirectory() || m_settings.num_backups == 0)
         {
            m_actions.push_back(CreateDeleteAction(source_file_info));
         }
         else
         {
            uint32_t backup_id = CreateBackupActionsForFile(file_info_it->second.GetName());

            std::string ext = file_name::GetExtension(file_info_it->second.GetName());
            std::string file_name = file_name::RemoveExtension(file_info_it->second.GetName());
            std::string backup_name = "$" + string::ToString(backup_id) + "$";
            file_name = file_name::AddFileNameAndExtension(file_name, backup_name);
            file_name = file_name::AddFileNameAndExtension(file_name, ext);

            FileInfo dest_file_info = file_info_it->second;
            dest_file_info.SetName(file_name::AddFileNameAndPath(file_name, m_settings.backup_folder));
            m_actions.push_back(CreateMoveAction(source_file_info, dest_file_info));
         }
      }
   }
}

