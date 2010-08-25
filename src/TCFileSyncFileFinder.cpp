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
//  $Id: TCFileSyncFileFinder.cpp 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************
#include "TCFileSyncFileFinder.h"

#include "TCFile.h"
#include "TCFileName.h"
#include "TCOutput.h"
#include "TCSystem.h"

#include "TCNewEnable.h"

TC::FileSync::FileFinder::FileFinder(const Settings& settings)
:m_settings(settings)
{
}

TC::FileSync::FileFinder::~FileFinder()
{
}

bool TC::FileSync::FileFinder::Find()
{
   std::string dir_save = File::GetDirectory();

   if (!FindSourceFiles()) return false;
   if (!FindDestinationFiles()) return false;

   return File::ChangeDirectory(dir_save);
}

bool TC::FileSync::FileFinder::FindSourceFiles()
{
   TCINFO1("FileSync::FileFinder", "Searching source files in %s ...", m_settings.source.c_str());

   m_files_source.clear();
   if (!File::ChangeDirectory(m_settings.source)) return false;
   if (!FindRecursiveFiles(".", m_files_source)) return false;

   TCINFO1("FileSync::FileFinder", "Searching source files in %s done.", m_settings.source.c_str());
   return true;
}

bool TC::FileSync::FileFinder::FindDestinationFiles()
{
   TCINFO1("FileSync::FileFinder", "Searching destination files in %s ...", m_settings.destination.c_str());

   // reset setting for destination search because we have to find all
   FileSync::Settings settings = m_settings;
   m_settings.extensions_to_search_for.clear();
   m_settings.extensions_to_skipp.clear();
   m_settings.folders_to_skipp.clear();

   m_files_destination.clear();
   if (!File::ChangeDirectory(m_settings.destination)) return false;
   if (!FindRecursiveFiles(".", m_files_destination)) return false;

   TCINFO1("FileSync::FileFinder", "Searching destination files in %s done.", m_settings.destination.c_str());

   m_settings = settings;

   return true;
}

bool TC::FileSync::FileFinder::FindRecursiveFiles(const std::string& search_dir, FileInfos& files)
{
   std::vector<File::FileInfo> file_infos_of_dir;
   File::GetFileInfosOfDirectory(file_infos_of_dir, search_dir);

   std::vector<File::FileInfo>::const_iterator file;
   for (file=file_infos_of_dir.begin(); file!=file_infos_of_dir.end(); file++)
   {
      FileInfo file_info(*file);
      file_info.SetName(FileName::AddFileNameAndPath(file->name, search_dir));
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
            std::string ext = FileName::GetExtension(file_info.GetName());
            if (m_settings.extensions_to_search_for.find(ext) == 
               m_settings.extensions_to_search_for.end())
            {
               continue;
            }
         }

         // check if we have to skipp some extensions
         if (m_settings.extensions_to_skipp.size())
         {
            std::string ext = FileName::GetExtension(file_info.GetName());
            if (m_settings.extensions_to_skipp.find(ext) != 
               m_settings.extensions_to_skipp.end())
            {
               continue;
            }
         }

         if (m_settings.calc_checksum)
         {
            file_info.CalculateHash();
         }
      }

      TCTRACE1("FileSync::FileFinder", 10, "Found file %s.", file_info.GetName().c_str());
      files[file_info.GetName()] = file_info;
   }

   return true;
}
