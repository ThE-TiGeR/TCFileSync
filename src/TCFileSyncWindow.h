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
//  $Id: TCFileSyncWindow.h,v 377861d4ecde 2012/04/24 22:25:51 thomas $
//*******************************************************************************
#ifndef _TC_FILE_SYNC_WINDOW_H_
#define _TC_FILE_SYNC_WINDOW_H_

#include "TCGuiApplication.h"
#include "TCFileSyncSettings.h"

namespace tc
{
   namespace file_sync
   {
      class Window: public FX::FXMainWindow
      {
         TCGUIBASE_DECLARE(Window);
      public:
         Window(gui::Application* appl, const Settings& settings);
         virtual ~Window();

         /** Enumarations for all fox messages for this class */
         enum WindowFXIDs
         {
            ID_ABOUT = FX::FXMainWindow::ID_LAST,
            ID_QUIT,
            ID_OPTIONS,

            ID_SOURCE_DIR,
            ID_SEL_SOURCE_DIR,
            ID_TARGET_DIR,
            ID_SEL_TARGET_DIR,

            ID_SYNC,

            ID_LAST
         };

         /** methode is called when we select exit in the menu */
         long OnCmdQuit(FX::FXObject*, FX::FXSelector, void*);
         long OnCmdAbout(FX::FXObject*, FX::FXSelector, void*);
         long OnCmdOptions(FX::FXObject*, FX::FXSelector, void*);

         long OnCmdSourceDir(FX::FXObject*, FX::FXSelector, void*);
         long OnCmdSelSourceDir(FX::FXObject*, FX::FXSelector, void*);
         long OnCmdTargetDir(FX::FXObject*, FX::FXSelector, void*);
         long OnCmdSelTargetDir(FX::FXObject*, FX::FXSelector, void*);
         long OnCmdSync(FX::FXObject*, FX::FXSelector, void*);

         long OnUpdSourceDir(FX::FXObject*, FX::FXSelector, void*);
         long OnUpdSelSourceDir(FX::FXObject*, FX::FXSelector, void*);
         long OnUpdTargetDir(FX::FXObject*, FX::FXSelector, void*);
         long OnUpdSelTargetDir(FX::FXObject*, FX::FXSelector, void*);
         long OnUpdSync(FX::FXObject*, FX::FXSelector, void*);

         virtual void create();

         FX::FXList* GetInfoList() {return m_info_list;}
      protected:
         Window() {}

      private:
         gui::Application* m_appl;
         Settings m_settings;
         FX::FXMenuPane* m_file_menu;
         FX::FXMenuPane* m_help_menu;
         FX::FXList* m_info_list;
         FX::FXLabel* m_progress_label;
         FX::FXProgressBar* m_progress_bar;
      };
   }
}

#endif // _TC_FILE_SYNC_WINDOW_H_
