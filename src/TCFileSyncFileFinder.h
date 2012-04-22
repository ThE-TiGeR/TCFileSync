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
#ifndef _TC_FILE_SYNC_FILE_FINDER_H_
#define _TC_FILE_SYNC_FILE_FINDER_H_

#include "TCFileSyncFileInfo.h"
#include "TCFileSyncSettings.h"

namespace tc
{
   namespace file_sync
   {
      class FileFinder
      {
      public:
         FileFinder(const Settings& settings);
         virtual ~FileFinder();

         bool Find();

         const FileInfos& GetSourceFiles() const
         {
            return m_files_source;
         }
         const FileInfos& GetDestinationFiles() const
         {
            return m_files_destination;
         }

      private:
         bool FindSourceFiles();
         bool FindDestinationFiles();
         bool FindRecursiveFiles(const std::string& directory, FileInfos& files);

         Settings m_settings;
         FileInfos m_files_source;
         FileInfos m_files_destination;
      };
   }
}

#endif // _TC_FILE_SYNC_FILE_FINDER_H_
