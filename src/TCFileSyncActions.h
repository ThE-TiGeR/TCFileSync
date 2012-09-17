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
#ifndef _TC_FILE_SYNC_ACTIONS_H_
#define _TC_FILE_SYNC_ACTIONS_H_

#include "TCFileSyncFileInfo.h"
#include "TCSharedPtr.h"

#include <string>
#include <vector> 

namespace tc
{
   namespace file_sync
   {
      class Action
      {
      public:
         virtual const std::string& GetActionString() const = 0;
         virtual const FileInfo* GetSource() const = 0;
         virtual const FileInfo* GetDestination() const = 0;
         virtual uint64_t GetBytesToSync() const = 0;

         virtual const std::string& GetErrorMessage() const = 0;

         virtual bool Do() = 0;

         virtual ~Action() {}
      };

      typedef SharedPtr<Action> ActionPtr;
      typedef std::vector<ActionPtr> Actions;

      ActionPtr CreateCopyAction(const FileInfo& source, const FileInfo& destination);
      ActionPtr CreateCreateDirectoryAction(const FileInfo& directory);
      ActionPtr CreateMoveAction(const FileInfo& source, const FileInfo& destination);
      ActionPtr CreateDeleteAction(const FileInfo& file);
    }
}

#endif // _TC_FILE_SYNC_SYNCHRONIZER_H_
