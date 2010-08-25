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
//  $Id: TCFileSyncMLS.cpp 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#include "TCMLSHandler.h"

#include "TCUtil.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace FileSync
   {
      MLS::Text m_tc_file_sync_gui_text_array[] =
      {
         TC::MLS::Text("TC_FILE_SYNC_FILE",           "en", "&File"),
         TC::MLS::Text("TC_FILE_SYNC_EXIT",           "en", "&Leave\tAlt-F4"),
         TC::MLS::Text("TC_FILE_SYNC_OPTIONS",        "en", "&Options"),
         TC::MLS::Text("TC_FILE_SYNC_HELP",           "en", "&?"),
         TC::MLS::Text("TC_FILE_SYNC_ABOUT",          "en", "&About"),

         TC::MLS::Text("TC_FILE_SYNC_SOURCE",          "en", "Source:"),
         TC::MLS::Text("TC_FILE_SYNC_TARGET",          "en", "Target:"),
         TC::MLS::Text("TC_FILE_SYNC_SYNCRONIZE",      "en", "Start Sync"),

         TC::MLS::Text("TC_FILE_SYNC_PREVIEW",         "en", "Preview"),
         TC::MLS::Text("TC_FILE_SYNC_NUM_BACKUPS",     "en", "Number of backups"),
         TC::MLS::Text("TC_FILE_SYNC_BACKUP_FOLDER",   "en", "Backup folder"),
      };

      MLS::Texts m_tc_file_sync_gui_texts(Util::ArraySize(m_tc_file_sync_gui_text_array),
         m_tc_file_sync_gui_text_array);
   }
}
