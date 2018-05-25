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
//  $Id: TCFileSyncSettings.h,v 5121974dd9eb 2012/09/17 19:34:55 thomas $
//*******************************************************************************
#ifndef _TC_FILE_SYNC_SETTINGS_H_
#define _TC_FILE_SYNC_SETTINGS_H_

#include "TCTypes.h"

#include <set>
#include <string>
#include <vector>

namespace tc
{
   namespace file_sync
   {
      class Settings
      {
      public:
         bool info_mode;
         bool calc_checksum;
         std::string source;
         std::string destination;

         uint32_t num_backups;
         std::string backup_folder;

         std::set< std::string > folders_to_skipp;
         std::set< std::string > extensions_to_search_for;
         std::set< std::string > extensions_to_skipp;
      };
   }
}

#endif // _TC_FILE_SYNC_SETTINGS_H_

