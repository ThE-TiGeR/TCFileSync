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
#include "TCFileSyncFileFinder.h"

#include "TCFile.h"
#include "TCFileName.h"
#include "TCOutput.h"
#include "TCSystem.h"

#include "TCNewEnable.h"

namespace tc
{
   file_sync::FileFinder::FileFinder(const Settings& settings)
      :m_settings(settings)
   {
   }

   file_sync::FileFinder::~FileFinder()
   {
   }

   bool file_sync::FileFinder::Find()
   {
      std::string dir_save = file::GetDirectory();

      if (!FindSourceFiles()) return false;
      if (!FindDestinationFiles()) return false;

      return file::ChangeDirectory(dir_save);
   }

   bool file_sync::FileFinder::FindSourceFiles()
   {
      TCINFOS("FileSync::FileFinder", "Searching source files in " << m_settings.source << " ...");

      m_files_source.clear();
      if (!file::ChangeDirectory(m_settings.source)) return false;
      if (!FindRecursiveFiles(".", m_files_source)) return false;

      FileInfos::iterator file_info;
      for (file_info=m_files_source.begin(); file_info!=m_files_source.end(); ++file_info)
      {
         file_info->second.SetDir(m_settings.source);
      }

      TCINFOS("FileSync::FileFinder", "Found " << m_files_source.size() << " source files in " << m_settings.source);
      return true;
   }

   bool file_sync::FileFinder::FindDestinationFiles()
   {
      TCINFOS("FileSync::FileFinder", "Searching destination files in " << m_settings.destination << " ...");

      // reset setting for destination search because we have to find all
      file_sync::Settings settings = m_settings;
      m_settings.extensions_to_search_for.clear();
      m_settings.extensions_to_skipp.clear();
      m_settings.folders_to_skipp.clear();

      m_files_destination.clear();
      if (!file::ChangeDirectory(m_settings.destination)) return false;
      if (!FindRecursiveFiles(".", m_files_destination)) return false;

      FileInfos::iterator file_info;
      for (file_info=m_files_destination.begin(); file_info!=m_files_destination.end(); ++file_info)
      {
         file_info->second.SetDir(m_settings.destination);
      }
      
      TCINFOS("FileSync::FileFinder", "Found " << m_files_destination.size() << " destination files in " << m_settings.destination);

      m_settings = settings;

      return true;
   }

   bool file_sync::FileFinder::FindRecursiveFiles(const std::string& search_dir, FileInfos& files)
   {
      std::vector<file::FileInfo> file_infos_of_dir;
      file::GetFileInfosOfDirectory(file_infos_of_dir, search_dir);

      std::vector<file::FileInfo>::const_iterator file;
      for (file=file_infos_of_dir.begin(); file!=file_infos_of_dir.end(); file++)
      {
         FileInfo file_info(*file);
         file_info.SetName(file_name::AddFileNameAndPath(file->name, search_dir));
         if (file_info.IsDirectory())
         {
            // check if we have to ignore this folder
            if (m_settings.folders_to_skipp.find(file->name) == m_settings.folders_to_skipp.end() &&
               file->name != m_settings.backup_folder)
            {
               FindRecursiveFiles(file_info.GetName(), files);
            }
            else
            {
               continue;
            }
         }
         else
         {
            // check if only some predefined extensions should be searched
            if (m_settings.extensions_to_search_for.size())
            {
               std::string ext = file_name::GetExtension(file_info.GetName());
               if (m_settings.extensions_to_search_for.find(ext) == 
                  m_settings.extensions_to_search_for.end())
               {
                  continue;
               }
            }

            // check if we have to skipp some extensions
            if (m_settings.extensions_to_skipp.size())
            {
               std::string ext = file_name::GetExtension(file_info.GetName());
               if (m_settings.extensions_to_skipp.find(ext) != 
                  m_settings.extensions_to_skipp.end())
               {
                  continue;
               }
            }
         }

         TCTRACES("FileSync::FileFinder", 10, "Found file " << file_info.GetName() << ".");
         files[file_info.GetName()] = file_info;
      }

      return true;
   }
}
