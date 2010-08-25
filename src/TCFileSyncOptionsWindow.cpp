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
//  $Id: TCFileSyncOptionsWindow.cpp 957 2010-01-28 23:17:00Z the_____tiger $
//*******************************************************************************

#include "TCFileSyncOptionsWindow.h"

#include "TCGuiLabel.h"
#include "TCMLSHandler.h"

#include "TCNewEnable.h"

namespace TC
{
   namespace FileSync
   {

      FXDEFMAP(OptionsWindow) OptionsWindowMap[]=
      {
         FXMAPFUNC(FX::SEL_COMMAND, OptionsWindow::ID_PREVIEW, OptionsWindow::OnCmdPreview),
         FXMAPFUNC(FX::SEL_UPDATE,  OptionsWindow::ID_PREVIEW, OptionsWindow::OnUpdPreview)
      };

      TCGUIBASE_IMPLEMENT(OptionsWindow, Gui::ActionBox, OptionsWindowMap, ARRAYNUMBER(OptionsWindowMap))

      OptionsWindow::OptionsWindow(FX::FXWindow* owner, const Settings& settings)
         :Gui::ActionBox(owner, "Options"),
         m_settings(settings)
      {
         closeOnExecute = true;
         FX::FXComposite* frame = GetPacker();
         FX::FXVerticalFrame* fv = new FX::FXVerticalFrame(frame, FX::LAYOUT_FILL_X | FX::LAYOUT_FILL_Y);

         FX::FXHorizontalFrame* fh = new FX::FXHorizontalFrame(fv, FX::LAYOUT_FILL_X);
         new FX::FXCheckButton(fh, MLS::Handler::GetText("TC_FILE_SYNC_PREVIEW").c_str(), this, ID_PREVIEW, FX::CHECKBUTTON_NORMAL | FX::LAYOUT_RIGHT);

         fh = new FX::FXHorizontalFrame(fv, FX::LAYOUT_FILL_X);
         new Gui::Label(fh, "TC_FILE_SYNC_NUM_BACKUPS");
         new FX::FXTextField(fh, 3, 0, 0, FX::TEXTFIELD_NORMAL|FX::LAYOUT_FILL_X);
         new Gui::Label(fh, "TC_FILE_SYNC_BACKUP_FOLDER");
         new FX::FXTextField(fh, 10, 0, 0, FX::TEXTFIELD_NORMAL|FX::LAYOUT_FILL_X);
      }

      long OptionsWindow::OnCmdPreview(FX::FXObject *obj, FX::FXSelector /*sel*/, void* /*ptr*/)
      {
         FX::FXint val = 0;

         obj->handle(this, MKUINT(FX::FXWindow::ID_GETINTVALUE, FX::SEL_COMMAND),
            &val);

         m_settings.info_mode = val ? true : false;
         return 0;
      }

      long OptionsWindow::OnUpdPreview(FX::FXObject *obj, FX::FXSelector /*sel*/, void* /*ptr*/)
      {
         FX::FXint val = m_settings.info_mode ? 1 : 0;

         obj->handle(this, MKUINT(FX::FXWindow::ID_SETINTVALUE, FX::SEL_COMMAND),
            &val);
         return 0;
      }
   }
}