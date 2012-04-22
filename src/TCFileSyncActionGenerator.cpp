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
// Copyright (C) 2003 - 2010 Thomas Goessler. All Rights Reserved. 
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
//  $Id$
//*******************************************************************************
#include "TCFileSyncActionGenerator.h"

#include "TCFile.h"
#include "TCFileName.h"
#include "TCOutput.h"
#include "TCString.h"

#include "TCNewEnable.h"

namespace tc
{
   namespace file_sync
   {
      ActionGenerator::ActionGenerator(const Settings& settings)
         :m_file_finder(settings),
         m_settings(settings)
      {

         m_settings.backup_folder = file_name::AddFileNameAndPath(settings.backup_folder, settings.destination);
      }

      bool ActionGenerator::CreateActions()
      {
         if (!m_file_finder.Find()) return false;

         TCINFO("FileSync", "Finding missing destination files ...");
         FileInfos missing_files = GetDifference(m_file_finder.GetSourceFiles(),
            m_file_finder.GetDestinationFiles());
         TCINFO("FileSync", "Finding missing destination files done.");

         TCINFO("FileSync", "Finding deleted source files ...");
         FileInfos deleted_files = GetDifference(m_file_finder.GetDestinationFiles(),
            m_file_finder.GetSourceFiles());
         TCINFO("FileSync", "Finding deleted source files done.");

         TCINFO("FileSync", "Finding modified source files ...");
         FileInfos modified_files = GetModifiedFiles(m_file_finder.GetSourceFiles(),
            m_file_finder.GetDestinationFiles());
         TCINFO("FileSync", "Finding modified source files done.");

         CreateMissingFileActions(missing_files);
         CreateDeletedFileActions(deleted_files);
         CreateModifiedFileActions(modified_files);

         return true;
      }

      FileInfos ActionGenerator::GetDifference(const FileInfos& files1,
         const FileInfos& files2)
      {
         FileInfos diff_files;

         FileInfos::const_iterator file;
         for (file=files1.begin(); file!=files1.end(); file++)
         {
            if (files2.find(file->second.GetName()) == files2.end())
            {
               diff_files.insert(*file);
            }
         }

         return diff_files;
      }

      FileInfos ActionGenerator::GetModifiedFiles(const FileInfos& files1,
         const FileInfos& files2)
      {
         FileInfos modified_files;

         uint64 num_checked = 0;
         uint64 total_files = files1.size();

         FileInfos::const_iterator file_it;
         for (file_it=files1.begin(); file_it!=files1.end(); file_it++, ++num_checked)
         {
            const FileInfo& file = file_it->second;
            // directories we do not need to check
            if (file.IsDirectory())
            {
               continue;
            }

            FileInfos::const_iterator found_file;
            found_file = files2.find(file.GetName());
            if (found_file != files2.end())
            {
               if (file.GetLastModified() > found_file->second.GetLastModified())
               {
                  modified_files[file.GetName()] = file;
               }
               else if (m_settings.calc_checksum)
               {
                  if ((num_checked % (total_files/100)) == 0)
                  {
                     TCINFOS("FileSync", num_checked << "(" << total_files << ")");
                  }

                  if (file.GetHashValue() != found_file->second.GetHashValue())
                  {
                     modified_files[file.GetName()] = file;
                  }
               }
            }
         }

         return modified_files;
      }

      void ActionGenerator::CreateMissingFileActions(const FileInfos& missing_files)
      {
         FileInfos::const_iterator file_info_it;
         for (file_info_it=missing_files.begin(); file_info_it!=missing_files.end(); file_info_it++)
         {
            FileInfo dest_file_info = file_info_it->second;
            dest_file_info.SetName(file_name::AddFileNameAndPath(file_info_it->second.GetName(), m_settings.destination));

            if (!dest_file_info.IsDirectory())
            {
               FileInfo source_file_info = file_info_it->second;
               source_file_info.SetName(file_name::AddFileNameAndPath(file_info_it->second.GetName(), m_settings.source));
               m_actions.push_back(CreateCopyAction(source_file_info, dest_file_info));
            }
         }
      }

      uint32 ActionGenerator::CreateBackupActionsForFile(const std::string& source_file_name)
      {
         std::string ext         = file_name::GetExtension(source_file_name);
         std::string file_name   = file_name::RemoveExtension(source_file_name);
         std::string path        = file_name::GetPath(source_file_name);

         uint32 backup_id = 1;
         Actions actions;
         for (uint32 file_idx=m_settings.num_backups; file_idx>0 ; file_idx--)
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

            std::string backup_id2 = "$" + string::ToString(file_idx-1) + "$";
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
         FileInfos::const_iterator file_info_it;
         for (file_info_it=modified_files.begin(); file_info_it!=modified_files.end(); file_info_it++)
         {
            FileInfo source_file_info = file_info_it->second;
            source_file_info.SetName(file_name::AddFileNameAndPath(file_info_it->second.GetName(), m_settings.source));

            FileInfo dest_file_info = file_info_it->second;
            dest_file_info.SetName(file_name::AddFileNameAndPath(file_info_it->second.GetName(), m_settings.destination));

            if (m_settings.num_backups > 0)
            {
               uint32 backup_id = CreateBackupActionsForFile(file_info_it->second.GetName());

               std::string ext         = file_name::GetExtension(file_info_it->second.GetName());
               std::string file_name   = file_name::RemoveExtension(file_info_it->second.GetName());
               std::string backup_name = "$" + string::ToString(backup_id) + "$";
               file_name               = file_name::AddFileNameAndExtension(file_name, backup_name);
               file_name               = file_name::AddFileNameAndExtension(file_name, ext);

               FileInfo move_file_info = file_info_it->second;
               move_file_info.SetName(file_name::AddFileNameAndPath(file_name, m_settings.backup_folder));

               m_actions.push_back(CreateMoveAction(dest_file_info, move_file_info));
            }

            m_actions.push_back(CreateCopyAction(source_file_info, dest_file_info));
         }
      }

      void ActionGenerator::CreateDeletedFileActions(const FileInfos& deleted_files)
      {
         FileInfos::const_reverse_iterator file_info_it;
         for (file_info_it=deleted_files.rbegin(); file_info_it!=deleted_files.rend(); file_info_it++)
         {
            FileInfo source_file_info = file_info_it->second;
            source_file_info.SetName(file_name::AddFileNameAndPath(file_info_it->second.GetName(), m_settings.destination));

            if (file_info_it->second.IsDirectory() || m_settings.num_backups == 0)
            {
               m_actions.push_back(CreateDeleteAction(source_file_info));
            }
            else
            {
               uint32 backup_id = CreateBackupActionsForFile(file_info_it->second.GetName());

               std::string ext         = file_name::GetExtension(file_info_it->second.GetName());
               std::string file_name   = file_name::RemoveExtension(file_info_it->second.GetName());
               std::string backup_name = "$" + string::ToString(backup_id) + "$";
               file_name               = file_name::AddFileNameAndExtension(file_name, backup_name);
               file_name               = file_name::AddFileNameAndExtension(file_name, ext);

               FileInfo dest_file_info = file_info_it->second;
               dest_file_info.SetName(file_name::AddFileNameAndPath(file_name, m_settings.backup_folder));
               m_actions.push_back(CreateMoveAction(source_file_info, dest_file_info));
            }
         }
      }
   }
}

