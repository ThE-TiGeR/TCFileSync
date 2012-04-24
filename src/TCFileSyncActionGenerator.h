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
#ifndef _TC_FILE_SYNC_ACTION_GENERATOR_H_
#define _TC_FILE_SYNC_ACTION_GENERATOR_H_

#include "TCFileSyncActions.h"
#include "TCFileSyncFileFinder.h"
#include "TCFileSyncSettings.h"
#include "TCFileSyncStatusDisplayer.h"

namespace tc
{
   namespace file_sync
   {
      class ActionGenerator
      {
      public:
         ActionGenerator(const Settings& settings, StatusDisplayerPtr status_displayer);

         bool CreateActions();
         const Actions& GetActions() const {return m_actions;}

      private:
         void FindMissingFiles();
         void FindDeletedFiles();
         void FindModifiedFiles();
         FileInfos GetDifference(const FileInfos& files1, const FileInfos& files2);
         FileInfos GetModifiedFiles(const FileInfos& files1, const FileInfos& files2);

         void CreateMissingFileActions(const FileInfos& missing_files);
         void CreateModifiedFileActions(const FileInfos& modified_files);
         void CreateDeletedFileActions(const FileInfos& deleted_files);

         uint32 CreateBackupActionsForFile(const std::string& source_file_name);

      private:
         FileFinder m_file_finder;
         Settings m_settings;
         Actions m_actions;
         StatusDisplayerPtr m_status_displayer;
      };
   }
}

#endif // _TC_FILE_SYNC_ACTION_GENERATOR_H_
