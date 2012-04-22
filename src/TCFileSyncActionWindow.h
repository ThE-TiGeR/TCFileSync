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
#ifndef _TC_FILE_SYNC_ACTION_WINDOW_H_
#define _TC_FILE_SYNC_ACTION_WINDOW_H_

#include "TCGuiApplication.h"
#include "TCFileSyncActions.h"

namespace tc
{
   namespace file_sync
   {
      class ActionWindow: public FX::FXTopWindow
      {
         TCGUIBASE_DECLARE(ActionWindow);
      public:
         ActionWindow(FX::FXWindow* window, const Actions& actions);
         virtual ~ActionWindow();

         /** Enumarations for all fox messages for this class */
         enum WindowFXIDs
         {
            ID_TABLE = FX::FXTopWindow::ID_LAST,
            ID_SYNC,

            ID_LAST
         };

         long OnCmdSync(FX::FXObject*, FX::FXSelector, void*);
         long OnUpdSync(FX::FXObject*, FX::FXSelector, void*);

         virtual void create();
      protected:
         ActionWindow() {}

      private:
         FX::FXTable* m_table;
      };
   }
}

#endif // _TC_FILE_SYNC_ACTION_WINDOW_H_
