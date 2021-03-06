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
//  $Id: TCFileSyncOptionsWindow.h,v 377861d4ecde 2012/04/24 22:25:51 thomas $
//*******************************************************************************

#ifndef _TC_FILE_SYNC_OPTIONS_WINDOW_H_
#define _TC_FILE_SYNC_OPTIONS_WINDOW_H_

#include "TCGuiActionBox.h"
#include "TCFileSyncSettings.h"

namespace tc
{
   namespace file_sync
   {

      /**
      * Class creates the property window for the chat settings
      */
      class OptionsWindow: public gui::ActionBox
      {
         TCGUIBASE_DECLARE(OptionsWindow);

      public:
         /// FXID`s need for widget message handling of this class
         enum OptionsWindowFXIDs
         {
            ID_PREVIEW = gui::ActionBox::ID_LAST,
            ID_CALC_CHECKSUM,
            ID_NUM_BACKUPS,
            ID_BACKUP_FOLDER,
            ID_FOLDERS_TO_SKIPP,
            ID_EXTENSIONS,
            ID_EXTENSIONS_TO_SKIPP,

            ID_LAST
         };

         OptionsWindow(FX::FXWindow* owner, const Settings& settings);

         const Settings& GetSettings() const {return m_settings;}

         long OnCmdPreview(FX::FXObject *obj, FX::FXSelector sel, void *ptr);
         long OnUpdPreview(FX::FXObject *obj, FX::FXSelector sel, void *ptr);
         long OnCmdCalcChecksum(FX::FXObject *obj, FX::FXSelector sel, void *ptr);
         long OnUpdCalcChecksum(FX::FXObject *obj, FX::FXSelector sel, void *ptr);
         long OnCmdNumBackups(FX::FXObject *obj, FX::FXSelector sel, void* ptr);
         long OnUpdNumBackups(FX::FXObject *obj, FX::FXSelector sel, void* ptr);
         long OnCmdBackupFolder(FX::FXObject *obj, FX::FXSelector sel, void* ptr);
         long OnUpdBackupFolder(FX::FXObject *obj, FX::FXSelector sel, void* ptr);

      protected:
         OptionsWindow() {}
      private:
         Settings m_settings;
      };
   }
}

#endif // _TC_FILE_SYNC_OPTIONS_WINDOW_H_
